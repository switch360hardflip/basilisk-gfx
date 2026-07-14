#include <basilisk-gfx.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/tile/bsgfx_tile.h>
#include <../bsgfx_contracts.h>
#include <basilisk-core.h>

bs_vec3 bsgfx_primitivePosition(bsgfx_RawPrimitive* primitive) {
    bs_vec4 q = bs_qFromDegrees(primitive->rotation);
    bs_vec3 rotated_scale = bs_qRotateVec3(&q, &primitive->scale);
    bs_vec3 result;
    bs_v3Add(&primitive->position, &rotated_scale, &result);
    return result;
}

static void bsgfx_mapPrimitive(bsgfx_RawPrimitive* unmapped, bsgfx_Primitive* mapped) {
    mapped->collision = unmapped->collision;
    mapped->guid = unmapped->guid;
    mapped->position = bsgfx_primitivePosition(unmapped);
    mapped->rotation = bs_qFromDegrees(unmapped->rotation);
    mapped->scale = unmapped->scale;
    mapped->flags = unmapped->flags;
    mapped->type = unmapped->type;

    switch (unmapped->type) {
    case BSGFX_PRIMITIVE_TYPE_BOX: mapped->subtype_index = BSGFX_SUBTYPE_PRIMITIVE_BOX; break;
    case BSGFX_PRIMITIVE_TYPE_SPHERE: mapped->subtype_index = BSGFX_SUBTYPE_PRIMITIVE_SPHERE; break;
    }
}

void bsgfx_loadPrimitives(int package_id) {
    bsgfx_type(
        BSGFX_TYPE_PRIMITIVE,
        package_id,
        BSGFX_PRIMITIVE_VERSION,
        "primitives", "primitive",
        sizeof(bsgfx_RawPrimitive), sizeof(bsgfx_Primitive), bsgfx_mapPrimitive,
        0, 0, 0, 0);

    if (bs_exists(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED)) {
        bs_Batch* mesh_instanced_batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED);

        if (bs_batchIsPushed(mesh_instanced_batch)) {
            bs_Range box_range = bs_pushCube(mesh_instanced_batch, BS_WHITE);
            bs_Range sphere_range = bs_pushSphere(mesh_instanced_batch, BS_V3(0, 0, 0), 1.0, 16, 16, BS_WHITE);
            bs_Range sphere_high_quality_range = bs_pushSphere(mesh_instanced_batch, BS_V3(0, 0, 0), 1.0, 64, 64, BS_WHITE);

            _bsgfx_subtypes_[BSGFX_SUBTYPE_PRIMITIVE_BOX] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, mesh_instanced_batch, BSGFX_SUBTYPE_HAS_SHADOWS, box_range);
            _bsgfx_subtypes_[BSGFX_SUBTYPE_PRIMITIVE_SPHERE] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, mesh_instanced_batch, BSGFX_SUBTYPE_HAS_SHADOWS, sphere_range);
        }
    }

    bs_U32 index = 0;

    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);

        if (primitive->type != BSGFX_PRIMITIVE_TYPE_BOX)
            continue;

        primitive->first_tile = index;
        for (int j = 0; j < 6; j++) {
            bsgfx_TileAxis* ax = &bsgfx_tileAxes()[j];

            int width = (int)(primitive->scale.a[ax->width_axis] * 2.0f);
            int height = (int)(primitive->scale.a[ax->height_axis] * 2.0f);

            index += width * height;
        }
        primitive->last_tile = index;
    }

   // bsgfx_instancePrimitives();
}

int bsgfx_primitiveSubtype(bsgfx_PrimitiveType type) {
    switch (type) {
    case BSGFX_PRIMITIVE_TYPE_BOX: return _bsgfx_subtypes_[BSGFX_SUBTYPE_PRIMITIVE_BOX];
    case BSGFX_PRIMITIVE_TYPE_SPHERE: return _bsgfx_subtypes_[BSGFX_SUBTYPE_PRIMITIVE_SPHERE];
    }

    return -1;
}

int bsgfx_instancePrimitive(int subtype, bs_mat4 transform, bs_U32 flags, int id, int material) {
    return bsgfx_instance(subtype, &transform, sizeof(bs_mat4), flags, 0, id, material);
}

int bsgfx_queryTilePrimitive(int tile_id) {
    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);
        if (tile_id >= primitive->first_tile && tile_id <= primitive->last_tile) {
            return i;
        }
    }

    bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Primitive for tile %d", tile_id);
    return -1;
}

void bsgfx_instancePrimitives() {
    if (!bs_exists(BSGFX_ATLASES, BSGFX_ATLAS_ANY))
        return;
    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;

    int white = $BSGFX_ATLAS_ANY_white()->id;
    bs_vec4 white_coords = bs_atlasCoordinates(atlas, white, 0);
    int red_material = $red_material()->id;

    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);
        if (primitive->flags & BSGFX_PRIMITIVE_HIDDEN)
            continue;

        bs_mat4 transform = bs_transform(primitive->position, primitive->rotation, primitive->scale);
        bs_U32 flags = BSGFX_ID_HIGHLIGHT | BSGFX_ID_IS_PRIMITIVE;

        //if (_bsgfx_procs_.bsmod_isSelected && _bsgfx_procs_.bsmod_isSelected(BSGFX_TYPE_PRIMITIVE, i))
        //    flags |= BSGFX_ID_SELECTED;

        //if (primitive->type == BSGFX_PRIMITIVE_TYPE_BOX) // tiles are rendered over these
        //    continue;

        bsgfx_instance(_bsgfx_subtypes_[primitive->subtype_index], &transform, sizeof(bs_mat4), flags, 0, i, 0);
    }
}

void bsgfx_renderPrimitives(bs_mat4 camera) {
    bs_PipelineHash hash;
    struct {
        bs_mat4 camera;
    } push_const = {
        .camera = camera,
    };

    hash = (bs_PipelineHash){
        .shaders = {
            $vs_bsgfx_mesh_instanced(),
            $fs_bsgfx_model(),
        },
        .stencil_front = {
            .fail_op = BS_STENCIL_OP_KEEP,
            .pass_op = BS_STENCIL_OP_KEEP,
            .depth_fail_op = BS_STENCIL_OP_KEEP,
            .compare_op = BS_COMPARE_OP_EQUAL,
            .compare_mask = 0xFF,
            .write_mask = 0x00,
            .reference = 2,
        },
    };
    bs_Pipeline* pipeline = bs_pipeline(&hash);

    bs_pushConstant(pipeline, 0, sizeof(&push_const), &push_const);
    bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_PRIMITIVE_BOX], pipeline);
    bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_PRIMITIVE_SPHERE], pipeline);
}

int bsgfx_queryPrimitive(bs_GUID* guid) {
    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);
        if (bs_sameGuid(guid, &primitive->guid))
            return i;
    }

    char guid_buffer[37];
    bs_guidToString(guid, guid_buffer);

    return -1;
}