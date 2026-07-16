#include <bs_types.h>
#include <bsmod_rasterize.h>
#include <bsmod_cache.h>
#include <bsmod_type.h>
#include <types/prefab/bsgfx_prefab.h>
#include <ui/bsgfx_ui.h>
#include <ui/bsmod_ui.h>
#include <ui/grid/bsmod_ui_grid.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/tile/bsgfx_tile.h>

void bsmod_pushPrefabMenuWidgets(bs_List* widgets, bs_vec2 background_size) {
    bs_vec3 offset = { 8 + 16, 0, 0 };
    offset.z += BSGFX_BACKGROUND_Z_COUNT;

    const int name_padding = 4;

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .advance_flags = BSGFX_WIDGET_ADVANCE_APPLY_OFFSET,
        .offset = { 0.0, -16.0, 0.0 },
    });

    static bs_vec3 temp = { 0 }, rot = { 0 }, sca = { 1, 1, 1 };
    const float input_width = 64.0;
    bsmod_pushVecNWidget(widgets, "Position", offset, input_width, &temp, 3);
    bsmod_pushVecNWidget(widgets, "Rotation", offset, input_width, &rot, 3);
    bsmod_pushVecNWidget(widgets, "Scale", offset, input_width, &sca, 3);
}

bool bsmod_instancePrefabPreview(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering) {
    bs_Model* model = bsgfx_prefabModel();
    bs_Atlas* atlas = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_UI)->head;

    if (bsmod_search_input->len > 0 && strstr(model->meshes[id].name, bsmod_search_input->value) == NULL) {
        return false;
    }

    bs_vec4 rotation = bs_qFromRadians(BS_V3(bs_radians(-45), bs_radians(-45), bs_radians(45)));

    const float scale = 1.0; // todo figure out AABB fitting with rotation instead
    bsgfx_instanceHiResMesh(
        model->meshes + id,
        BS_V3(position->x + widget->grid.size.x / 2.0, position->y + widget->grid.size.x / 2.0, 0),
        //bsgfx_adjustPosition(BS_V3(position->x + widget->grid.size.x, position->y, 13), hovering, id),
        rotation,
        widget->grid.size.x * scale,
        BSGFX_PREFAB_SUBTYPE_MESH_HI_RES_SCREEN,
        false);

    return true;
}



  /*==============================================================================
   * Dragging Icon
   *============================================================================*/

void bsmod_onDragPrefab(bsmod_DraggingParams params) {
    if (_bsmod_.hovering.tile < 0 || _bsmod_.hovering.primitive < 0)
        return;

    bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, _bsmod_.hovering.primitive);

    bs_ivec2 coords = bsgfx_tileCoordinate(primitive, _bsmod_.hovering.tile_axis, _bsmod_.hovering.tile);

    bs_vec3 tile_position = bsgfx_tilePosition(primitive, _bsmod_.hovering.tile_axis, coords.x, coords.y);
    bs_vec4 rotation = bsgfx_tileRotation(_bsmod_.hovering.tile_axis);
    bs_vec3 euler_rotation = bsgfx_tileEulerRotation(_bsmod_.hovering.tile_axis);
    rotation = bs_qMulq(primitive->rotation, rotation);
    bs_mat4 matrix = bs_transform(tile_position, rotation, bs_v3V1(1.0));

    bsgfx_instancePrefabModel(_bsmod_.dragging_id, matrix, BSGFX_PREFAB_SUBTYPE_MESH, $bsmod_light_blue()->id);
    bs_Model* prefab_model = bsgfx_prefabModel();
    bs_Mesh* prefab_mesh = prefab_model->meshes + _bsmod_.dragging_id;

    if (bs_leftClickUpOnce()) {
        bsgfx_RawPrefab* prefab = bsmod_add(BSGFX_TYPE_PREFAB, &(bsgfx_RawPrefab) {
            .position = tile_position,
            .rotation = euler_rotation,
            .scale = bs_v3V1(1),
            .name_hash = prefab_mesh->name_hash,
        });
        bsmod_saveType(BSGFX_TYPE_PREFAB, "Created prefab");
    }
}


  /*==============================================================================
   * Rasterizing Icons
   *============================================================================*/

void bsmod_rasterizePrefabIcons() {
#define WIDTH (84)
    const bs_ivec2 render_size = { WIDTH * 2, WIDTH * 2 };
    const bs_ivec2 output_size = { WIDTH, WIDTH };
#undef WIDTH

    bs_PipelineHash pipeline_hash = {
        .shaders = {
            $vs_bsmod_mesh_instanced(),
            $fs_bsmod_rasterize(),
        },
    };

    struct {
        bs_mat4x3 camera;
        bs_mat4x3 model;
        bs_vec4 color;
        int material_texture_size;
        int material_texture_id;
    } push_const = {
        .camera = bs_m4x3(bs_m4Mul(
            bs_ortho(0, render_size.x, 0, render_size.y, -500.0, 500.0),
            bs_lookAt(BS_V3(0, 0, 1), BS_V3(0, 0, 0), BS_V3(0, 1, 0))
        )),
    };

    int instance = 0;

    bsmod_beginRasterize(render_size, output_size);

    bsgfx_Material* material = $bsmod_light_blue();

    bs_Model* model = bsgfx_prefabModel();

    int count = model->meshes_count;
    bs_vec4 rotation = bs_qFromDegreesV3(15.0, 30.0, -15.0);

    for (int i = 0; i < count; i++) {
        bs_Mesh* mesh = model->meshes + i;
        int subtype = mesh->extra[BSGFX_MODEL_SUBTYPE_DEFAULT];

        bs_mat4 transform = bs_fitAabb(mesh->aabb, BS_IV2_TO_V2(render_size), rotation);

        push_const.color = material->contract->color;
        push_const.material_texture_size = material->contract->image_binding;
        push_const.material_texture_id = material->contract->image;
        push_const.model = bs_m4x3(transform);

        instance = bsgfx_instancePrefabModel(i, transform, BSGFX_PREFAB_SUBTYPE_MESH, $bsmod_light_blue()->id);
        bsmod_rasterizeInstance(pipeline_hash, subtype, instance, material->category, material->name, render_size.x, render_size.y, sizeof(push_const), &push_const);
    }

    bsmod_endRasterize();
}