#include <basilisk-core.h>

#include <types/prefab/bsgfx_prefab.h>
#include <types/tile/bsgfx_tile.h>

#include <bsgfx.h>

static bsgfx_PrefabMetadata* bsgfx_prefabs = NULL;
static int bsgfx_prefabs_count = 0;

static int bs_comparePrefabZ(const int* a_id, const int* b_id) {
    bsgfx_Type* type = bsgfx_types + BSGFX_TYPE_PREFAB;
    bsgfx_Prefab* a = type->mapped + *a_id;
    bsgfx_Prefab* b = type->mapped + *b_id;

    if (a->position.z == b->position.z) return 0;
    else if (a->position.z < b->position.z) return -1;
    else return 1;
}

static int bs_compareRawPrefabZ(const int* a_id, const int* b_id) {
    bsgfx_Type* type = bsgfx_types + BSGFX_TYPE_PREFAB;
    bsgfx_RawPrefab* a = type->unmapped + *a_id;
    bsgfx_RawPrefab* b = type->unmapped + *b_id;

    if (a->position.z == b->position.z) return 0;
    else if (a->position.z < b->position.z) return -1;
    else return 1;
}

enum {
    BSGFX_SHADOW_COMPUTATION_MESH,
    BSGFX_SHADOW_COMPUTATION_BONE,
    BSGFX_SHADOW_COMPUTATION_MESH_TYPE,
    BSGFX_SHADOW_COMPUTATION_MESH_TEXTURED,
};

static inline void bsgfx_awaitComputeWrite() {
    bs_barrier(0,
        BS_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        BS_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        BS_ACCESS_SHADER_WRITE_BIT,
        BS_ACCESS_SHADER_READ_BIT);
}

void bsgfx_computePrefabShadows() {
    if (!_bsgfx_prefab_model)
        return;
    return;

    bs_Buffer* b = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
    bsgfx_InstanceMetadata* m = bs_bufferMap(b);

    bs_setBufferAsync(
        bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer,
        offsetof(bsgfx_InstanceMetadata, computed_volume_vertices),
        BSGFX_NUM_VOLUME_COMPUTATIONS * sizeof(unsigned int), 0);

    // Reset shadow geometry
    bs_Object* prefab_shadows = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_PREFAB_SHADOWS);

    bs_copyAsync(
        prefab_shadows->batch->staging_buffer->buffer,
        prefab_shadows->batch->vertex_buffer->buffer,
        0, 0, BS_U32_MAX);

    bs_barrier(0,
        BS_PIPELINE_STAGE_TRANSFER_BIT,
        BS_PIPELINE_STAGE_COMPUTE_SHADER_BIT,
        BS_ACCESS_TRANSFER_WRITE_BIT,
        BS_ACCESS_SHADER_READ_BIT);

    struct {
        bs_vec3 light_direction;
        int id;
        int subtype_offset;
        int subtype_count;
        int include_flag;
        int exclude_flag;
    } push_constant = { 0 };

    push_constant.light_direction = _poser->sun_direction;
    push_constant.subtype_offset = 0;
    push_constant.subtype_count = -1;

    push_constant.id = BSGFX_SHADOW_COMPUTATION_MESH;
    push_constant.include_flag = BSGFX_SUBTYPE_HAS_SHADOWS;
    push_constant.exclude_flag = BSGFX_SUBTYPE_HAS_FINE_SHADOWS;

    bs_Pipeline* pipeline = bs_computePipeline($cs_bsgfx_mesh_volume(), 0);
    bsgfx_awaitComputeWrite();
    bs_pushConstant(pipeline, 0, sizeof(push_constant), &push_constant);
    bs_except(BSX_GENERAL);
    bs_dispatchAsync(pipeline, 1, 1, 1);
    bs_except(0);

}
#define BSGFX_STENCIL_SHADOW_VOLUME_OPTIONS                                             \
    .stencil_front = {                                                                  \
        .depth_fail_op = BS_STENCIL_OP_INCREMENT_AND_WRAP,                              \
        .compare_op = BS_COMPARE_OP_ALWAYS,                                             \
        .compare_mask = 0xFF,                                                           \
        .write_mask = 0xFF,                                                             \
    },                                                                                  \
    .stencil_back = {                                                                   \
        .depth_fail_op = BS_STENCIL_OP_DECREMENT_AND_WRAP,                              \
        .compare_op = BS_COMPARE_OP_ALWAYS,                                             \
        .compare_mask = 0xFF,                                                           \
        .write_mask = 0xFF,                                                             \
    },                                                                                  \
    .cull_type = BS_CULL_MODE_NONE,                                                     \
    .clamp_depth = true,                                                                \
    .skip_depth_write = true,                                                           \
    .attachments = {                                                                    \
        [0] = {.skip_write = true },                                                    \
        [3] = {.skip_write = true },                                     /* position */ \
    }                                                                                   \

void bsgfx_renderPrefabShadowVolumes() {
    if (!bs_exists(BSGFX_BATCHES, BSGFX_BATCH_PREFAB_SHADOWS))
        return;

    bs_beginComment("Shadow Volumes");

    bs_Pipeline* volume_pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_volume(),
            $fs_bsgfx_volume(),
        },
        BSGFX_STENCIL_SHADOW_VOLUME_OPTIONS
    });

    bs_Pipeline* volume_screen_pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_color_percentage(),
            $fs_bsgfx_volume_screen(),
        },
        .skip_depth_write = true,
        .stencil_front = {
            .compare_op = BS_COMPARE_OP_NOT_EQUAL,
            .compare_mask = 0xFF,
        },
        .attachments = {
            [3] = {.skip_write = true }, // position
        }
    });

    bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
    bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

    int count = metadata->computed_volume_vertices[BSGFX_SHADOW_COMPUTATION_MESH];

    if (count > BSGFX_NUM_SHADOW_VERTICES)
        bs_throwBasiliskF(BSX_OUT_OF_BOUNDS, "Compute shader generated %d/%d vertices", count, BSGFX_NUM_SHADOW_VERTICES);

    bs_pushConstant(volume_pipeline, 0, sizeof(_poser->camera.result), &_poser->camera.result);
    bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_PREFAB_SHADOWS)->batch, volume_pipeline, 0, count, 0, 1);

    bs_barrier(0,
        BS_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
        BS_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        BS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
        BS_ACCESS_SHADER_READ_BIT);

    bs_pushConstant(volume_screen_pipeline, 0, sizeof(bs_vec4), &BSGFX_RGBA(BSGFX_SHADOW_COLOR.r, BSGFX_SHADOW_COLOR.g, BSGFX_SHADOW_COLOR.b, BSGFX_SHADOW_COLOR.a));
    bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_VOLUME_SCREEN)->batch, volume_screen_pipeline, 0, BS_U32_MAX, 0, 1);

    bs_endComment();
}

#ifdef _DEBUG
void bsgfx_savePrefabs() {
    //qsort(bsgfx_types[BSGFX_TYPE_PREFAB].mapped_accessors, bsgfx_types[BSGFX_TYPE_PREFAB].count, sizeof(int), bs_comparePrefabZ);
    //qsort(bsgfx_types[BSGFX_TYPE_PREFAB].unmapped_accessors, bsgfx_types[BSGFX_TYPE_PREFAB].count, sizeof(int), bs_compareRawPrefabZ);

    /*
    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
    bs_Model* model = _bsgfx_prefab_model;
    bs_Json metadata = bs_emptyJson();
    for (int i = 0; i < bsgfx_prefabs_count; i++) {
        bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, i);

        for (int j = 0; j < bsgfx_prefabs[i].variety_count; j++) {
            double texture_hash_double;
            assert(sizeof(bs_U64) == sizeof(double));
            memcpy(&texture_hash_double, &atlas->unmapped[bsgfx_prefabs[i].varieties[j].texture_id].name_hash, sizeof(bs_U64));
            bs_ensureJsonF(&metadata, bs_jsonValue(texture_hash_double), "%s.varieties[9999].textureHash", model->meshes[i].name);
        }
    }

    char* raw = bs_saveJson(&metadata, BS_JSON_PRETTY);
    bs_saveFile("resources/meshes.json", raw, strlen(raw));

    bs_destroyJson(&metadata);
    bs_free(raw);
    */
}
#endif

static void bsgfx_mapPrefab(bsgfx_RawPrefab* unmapped, bsgfx_Prefab* mapped) {
    bs_Model* model = _bsgfx_prefab_model;
    bs_except(BSX_FAILED_TO_QUERY);
    bs_Mesh* mesh = bs_queryMeshHash(model, unmapped->name_hash, NULL);
    bs_except(0);

    int mesh_id = mesh ? mesh - model->meshes : -1;

    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;

    bs_except(BSX_FAILED_TO_QUERY);
    bsgfx_Material* material = unmapped->material_hash ? bsgfx_queryMaterialHash(unmapped->material_hash) : 0;
    bs_except(0);

    *mapped = (bsgfx_Prefab){
        .position = unmapped->position,
        .rotation = bs_qFromDegrees(unmapped->rotation),
        .material_id = material ? material->id : 0,
        .mesh_id = mesh_id,
        .scale = unmapped->scale,
        .guid = unmapped->guid,
        .texture_id = unmapped->texture_hash ? bs_queryAtlasHash(atlas, unmapped->texture_hash, NULL) : -1,
        .z_order = unmapped->z_order,
        .flags = unmapped->flags,
        .shadow_subtype = -1,
    };
}

static void bsgfx_destroyPrefabVarieties() {
    bs_Model* model = _bsgfx_prefab_model;


    if (bsgfx_prefabs) {
        for (int i = 0; i < bsgfx_prefabs_count; i++) {
            if (bsgfx_prefabs[i].varieties)
                bs_free(bsgfx_prefabs[i].varieties);
        }
        bs_free(bsgfx_prefabs);
    }
}

/*
static void bsgfx_loadPrefabVarieties() {
    bs_Model* model = _bsgfx_prefab_model;
    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
    bs_Json json = bs_loadJson("resources/meshes.json");

    bsgfx_prefabs = bs_calloc(model->meshes_count, sizeof(*bsgfx_prefabs));
    bsgfx_prefabs_count = model->meshes_count;

    for (int i = 0; i < model->meshes_count; i++) {
        bs_JsonValue mesh = bs_fetchJson(&json, BS_JSON_UNDEFINED, model->meshes[i].name);
        if (mesh.type != BS_JSON_OBJECT)
            continue;

        bs_Json root = bs_jsonRoot(&json, mesh.as_object);
        bs_JsonValue varieties = bs_fetchJson(&root, BS_JSON_ARRAY, "varieties");

        bsgfx_prefabs[i].varieties = bs_malloc(varieties.size * sizeof(bsgfx_PrefabVariety));
        bsgfx_prefabs[i].variety_count = varieties.size;

        for (int j = 0; j < varieties.size; j++) {
            bs_Json variety = bs_jsonRoot(&root, varieties.as_array.as_objects[j]);
            double texture_hash_double = bs_fetchJson(&variety, BS_JSON_NUMBER, "textureHash").as_number;
            bs_U64 texture_hash;
            memcpy(&texture_hash, &texture_hash_double, sizeof(bs_U64));

            bsgfx_prefabs[i].varieties[j] = (bsgfx_PrefabVariety){
                .texture_id = bs_queryAtlasHash(atlas, texture_hash, NULL),
            };
        }
    }

    bs_destroyJson(&json);
}
*/

bsgfx_PrefabMetadata* bsgfx_prefabMetadata(int mesh_id) {
    return &bsgfx_prefabs[mesh_id];
}

void bsgfx_loadPrefabs(int package_id, bs_Model* model) {
    assert(model);

    if (!bs_exists(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED) || !bs_exists(BSGFX_ATLASES, BSGFX_ATLAS_ANY))
        return;

    bs_Batch* batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED)->batch;

    _bsgfx_prefab_model = model;

    bs_Batch* volume_batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_MESH_TYPE_VOLUME_COMPUTED)->batch;
    bsgfx_destroyPrefabVarieties();

    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;

    if (!bs_batchIsPushed(batch)) {
        bs_except(BSX_DUPLICATE);

        static int* cached_subtypes;
        static int cached_subtypes_count;
        int cached_subtype_offset = 0;

        if (!cached_subtypes) {
            cached_subtypes_count = model->meshes_count * BSGFX_PREFAB_SUBTYPE_COUNT + model->primitives_count + 1; // + 1 for the whole model
            cached_subtypes = bs_malloc(cached_subtypes_count * sizeof(int));
            memset(cached_subtypes, -1, cached_subtypes_count * sizeof(int));
        }

        model->extra = cached_subtypes + cached_subtype_offset++;

        for (int i = 0; i < model->meshes_count; i++) {
            bs_Mesh* mesh = model->meshes + i;
            bs_Range range = { .offset = batch->indices.count, .batch = batch };

            for (int j = 0; j < mesh->primitives_count; j++) {
                bs_Primitive* primitive = mesh->primitives + j;

                bs_Range primitive_range = bs_pushPrimitive(batch, primitive);
                range.num += primitive_range.num;

                primitive->extra = cached_subtypes + cached_subtype_offset++;
                if (primitive->extra[BSGFX_PREFAB_SUBTYPE_PRIMITIVE] >= 0)
                    bsgfx_deleteSubtype(primitive->extra[BSGFX_PREFAB_SUBTYPE_PRIMITIVE]);

                primitive->extra[BSGFX_PREFAB_SUBTYPE_PRIMITIVE] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH_STATIC, batch, 0, primitive_range);
            }

            mesh->extra = cached_subtypes + cached_subtype_offset;
            for (int j = 0; j < BSGFX_PREFAB_SUBTYPE_COUNT; j++) {
                int subtype = mesh->extra[BSGFX_PREFAB_SUBTYPE_MESH];
                if (subtype >= 0)
                    bsgfx_deleteSubtype(subtype);
            }

            mesh->extra[BSGFX_PREFAB_SUBTYPE_MESH] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH_STATIC, batch, BSGFX_SUBTYPE_HAS_SHADOWS, range);
            mesh->extra[BSGFX_PREFAB_SUBTYPE_MESH_POLYGON_OUTLINE] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, batch, 0, range);
            mesh->extra[BSGFX_PREFAB_SUBTYPE_MESH_TEXTURED] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, batch, BSGFX_SUBTYPE_HAS_SHADOWS | BSGFX_SUBTYPE_HAS_FINE_SHADOWS, range);
            mesh->extra[BSGFX_PREFAB_SUBTYPE_MESH_HI_RES_SCREEN] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, batch, 0, range);
            mesh->extra[BSGFX_PREFAB_SUBTYPE_MESH_LO_RES_SCREEN] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, batch, 0, range);
            mesh->extra[BSGFX_PREFAB_SUBTYPE_MESH_VOLUME] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH_STATIC, volume_batch, 0, (bs_Range) { .batch = volume_batch });
            cached_subtype_offset += BSGFX_PREFAB_SUBTYPE_COUNT;

        }

        // sktchy
//        bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, BSGFX_MESH_KEY_SPHERE, BSGFX_BATCH_MESH_INSTANCED, 0, bs_pushSphere(batch, bs_v3V1(0), 1.0, 8, 8, BS_WHITE), true);
//        bs_Model* skateboard = bs_fetch(BS_SOURCE_GFX, BSGFX_MODEL_SKATEBOARD).model;
//        bs_Range skateboard_range = bs_pushMesh(batch, bs_queryMesh(skateboard, "board"));
//        bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, BSGFX_MODEL_SKATEBOARD, BSGFX_BATCH_MESH_INSTANCED, bsgfx_subtypes[BSGFX_SUBTYPE_HAS_SHADOWS, skateboard_range, true);
//        bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, BSGFX_VOLUME_DOWN_KEY + BSGFX_MODEL_SKATEBOARD, BSGFX_BATCH_MESH_INSTANCED, bsgfx_subtypes[BSGFX_SUBTYPE_HAS_SHADOWS, skateboard_range, true);
//        bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, BSGFX_MESH_KEY_SKATEBOARD_WHEELS, BSGFX_BATCH_MESH_INSTANCED, bsgfx_subtypes[BSGFX_SUBTYPE_HAS_SHADOWS, bs_pushMesh(batch, bs_queryMesh(skateboard, "wheels")), true);

        bs_Quad plane = bs_quad(bs_v3(0, 0, 0), bs_v2(1, 1));
        _bsgfx_subtypes[BSGFX_SUBTYPE_PLANE_MESH] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, batch, BSGFX_SUBTYPE_HAS_SHADOWS, bs_pushQuad(batch, plane, BS_WHITE));
        bs_except(0);
    }

    bs_except(BSX_FAILED_TO_QUERY);
    bsgfx_type(
        BSGFX_TYPE_PREFAB,
        package_id,
        BSGFX_PREFAB_VERSION,
        "prefabs", "prefab",
        sizeof(bsgfx_RawPrefab), sizeof(bsgfx_Prefab), bsgfx_mapPrefab,
        0, 0, 0, 0);
    bs_except(0);

   // bsgfx_instancePrefabs();

    //bsgfx_loadPrefabVarieties();

}

bs_mat4 bsgfx_prefabTransform(bsgfx_Prefab* prefab) {
    bs_mat4 m = BS_MAT4_IDENTITY;

    bs_translateP(&m, &prefab->position);
    bs_rotateP(&m, &prefab->rotation);
    bs_scaleP(&m, &prefab->scale);

    return m;
}

int bsgfx_instancePrefabModel(int mesh_id, bs_mat4 transform, bsgfx_PrefabSubtype prefab_subtype, int material_id) {
    bs_Mesh* mesh = _bsgfx_prefab_model->meshes + mesh_id;
    int subtype = mesh->extra[prefab_subtype];
    return bsgfx_instance(subtype, &transform, sizeof(bs_mat4), BSGFX_ID_IS_PREFAB, 0, 0, material_id);
}

int bsgfx_instancePrefab(int id, bsgfx_PrefabSubtype prefab_subtype) {
    assert(_bsgfx_prefab_model);

    bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, id);
    bsgfx_RawPrefab* raw_prefab = bsgfx_getRaw(BSGFX_TYPE_PREFAB, id);

    if (prefab->mesh_id < 0)
        return -1;

    bs_Mesh* mesh = _bsgfx_prefab_model->meshes + prefab->mesh_id;
    bs_mat4 transform = bsgfx_prefabTransform(prefab);
    int subtype = mesh->extra[prefab_subtype];
    int material = prefab->material_id;
    int texture = prefab->texture_id;
    int quad_instance_offset = -1;

    bs_U32 flags = BSGFX_ID_IS_PREFAB;

    if (texture >= 0) {
        bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;

        bs_vec2 size = bs_atlasSize(atlas, texture);
        bs_mat4 quad_transform = bs_transform(bs_v3(
            prefab->position.x * BSGFX_TILE_SIZE.x / 2.0,
            -(prefab->position.z - prefab->position.y) * BSGFX_TILE_SIZE.y / 2.0,
            prefab->position.z
        ), BS_QUAT_IDENTITY, bs_v3(size.x, size.y, 0.0));

        if (prefab_subtype != BSGFX_PREFAB_SUBTYPE_MESH_POLYGON_OUTLINE) { // theres betterr .. yeah
            bs_vec4 coords = bs_atlasCoordinates(atlas, texture, 0);
            if (!(raw_prefab->flags & BSGFX_PREFAB_HIDDEN)) {
                bs_mat4x3 mat = bs_m4x3(quad_transform);
                if (raw_prefab->flags & BSGFX_PREFAB_WRITE_POSITION)
                    quad_instance_offset = bsgfx_instanceQuad(_bsgfx_subtypes[BSGFX_SUBTYPE_ATLAS_PREFAB_TRANSPARENT], mat, coords, flags, id, 0);
                else
                    quad_instance_offset = bsgfx_instanceQuad(_bsgfx_subtypes[BSGFX_SUBTYPE_ATLAS_PREFAB_TRANSPARENT], mat, coords, flags, id, 0);
            }
        }
    }

    //if (_bsgfx_procs.bsmod_isSelected && _bsgfx_procs.bsmod_isSelected(BSGFX_TYPE_PREFAB, id))
    //    flags |= BSGFX_ID_SELECTED;
    return bsgfx_instance(subtype, &transform, sizeof(bs_mat4), flags, quad_instance_offset, id, material);
}

void bsgfx_instancePrefabs() {
    int count = bsgfx_count(BSGFX_TYPE_PREFAB);
    if (count <= 0) return;

    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
    bs_Model* model = _bsgfx_prefab_model;

    for (int i = 0; i < count; i++) {
        bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, i);

        bsgfx_PrefabSubtype subtype = BSGFX_PREFAB_SUBTYPE_MESH;
        if (prefab->flags & BSGFX_PREFAB_TEXTURED_SHADOWS)
            subtype = BSGFX_PREFAB_SUBTYPE_MESH_TEXTURED;
        bsgfx_instancePrefab(i, subtype);
    }
}

void bsgfx_renderPrefabs(bs_Pipeline* pipeline, int key_start) {
    assert(_bsgfx_prefab_model);
    bs_Model* model = _bsgfx_prefab_model;

    for (int i = 0; i < model->meshes_count; i++) {
        bs_Mesh* mesh = model->meshes + i;
        if (!mesh->extra)
            continue;
        int subtype = mesh->extra[key_start];
        bsgfx_renderSubtype(subtype, pipeline);
    }
}

void bsgfx_renderScenePrefabs() {
    if (!_bsgfx_prefab_model)
        return;

    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;

    struct {
        bs_mat4 camera;
        bs_vec4 uv;
        bs_vec4 sun_direction;
    } mesh_push_const = {
        .camera = _poser->camera.result,
        .uv = bs_atlasCoordinates(atlas, bs_queryAtlas(atlas, "white"), 0),
        .sun_direction.xyz = _poser->sun_direction,
    };

    bs_PipelineHash hash = {
        .shaders = {
            $vs_bsgfx_mesh_static_instanced(),
            $fs_bsgfx_model(),
        },
        // BSGFX_REQUIRED_FOR_SHADOW_VOLUMES,
        .cull_type = _bsgfx_settings.cull_backfaces ? BS_CULL_MODE_DEFAULT : BS_CULL_MODE_NONE,
        //.attachments[0].skip_write = true,
        //  .cull_type = BS_CULL_MODE_NONE,
        // .attachments = {
        //      [0] = {.skip_write = true},
        // }
    };
    // TODO: screenshot reimplementation
    //if (bsmod.queue.screenshot)
    //    hash.attachments[0].skip_write = false;
    bs_Pipeline* mesh_pipeline = bs_pipeline(&hash);

    // todo debug
//           if (bsgfx_debug.queue.skip_mesh_index_write) {
//               hash.attachments[bsgfx_queryLoResImageIndex(BSGFX_IMAGE_LO_RES_0_INDEX)].skip_write = true;
//               mesh_pipeline = bs_pipeline(&hash);
//           }

    bs_beginComment("Prefabs");
    bs_pushConstant(mesh_pipeline, 0, sizeof(mesh_push_const), &mesh_push_const);
    bsgfx_renderPrefabs(mesh_pipeline, BSGFX_PREFAB_SUBTYPE_MESH);
    bs_endComment();
}

void bsgfx_renderPrefabPrimitives(bs_Pipeline* pipeline, int key_start) {
    if (bsgfx_count(BSGFX_TYPE_PREFAB) <= 0)
        return;

    bs_Model* model = _bsgfx_prefab_model;

    for (int i = 0; i < model->meshes_count; i++) {
        bs_Mesh* mesh = model->meshes + i;

        for (int j = 0; j < mesh->primitives_count; j++) {
            bs_Primitive* primitive = mesh->primitives + j;
            int subtype = primitive->extra[key_start];
            bsgfx_renderSubtype(subtype, pipeline);
        }
    }
}

int bsgfx_queryPrefabId(const bs_GUID* guid) {
    int id = -1;
    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PREFAB); i++) {
        bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, i);
        if (bs_sameGuid(guid, &prefab->guid)) {
            id = i;
            break;
        }
    }

    if (id == -1) {
        char string_guid[37];
        bs_guidToString(guid, string_guid);
        bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Mesh %s\n", string_guid);
    }

    return id;
}

int bsgfx_closestPrefab(bs_U64 mesh_name_hash, bs_vec3 position, float radius) {
    bs_Model* model = _bsgfx_prefab_model;

    // todo use sweep data somehow
    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PREFAB); i++) {
        bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, i);
        bs_Mesh* mesh = model->meshes + prefab->mesh_id;

        if (mesh->name_hash != mesh_name_hash)
            continue;

        if (bs_v3Dist(prefab->position, position) < radius)
            return i;
    }

    return -1;
}
