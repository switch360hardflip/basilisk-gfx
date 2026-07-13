#include <bs_types.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/tile/bsgfx_tile.h>
#include <ui/bsgfx_ui.h>
#include <ui/bsmod_ui.h>
#include <bsmod_rasterize.h>
#include <bsgfx_cache.h>
#include <bsmod_cache.h>
#include <bsmod_type.h>

bool bsmod_instancePrimitivePreview(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering) {
    float scale = 0.25; // todo figure out AABB fitting with rotation instead

    bs_vec4 rotation = bs_qFromRadians(BS_V3(bs_radians(-45), bs_radians(-45), bs_radians(45)));
    bs_mat4 transform = bs_transform(BS_V3(position->x + widget->grid.size.x / 2.0, position->y + widget->grid.size.y / 2.0, 0.0), rotation, bs_v3V1(widget->grid.size.x * scale));

    int subtype = bsgfx_primitiveSubtype(id);
    bsgfx_instancePrimitive(subtype, transform, 0, 0, 0);

    return true;
}



  /*==============================================================================
   * Dragging Icon
   *============================================================================*/

void bsmod_onDragPrimitive(bsmod_DraggingParams params) {
    if (bsmod.hovering.tile < 0 || bsmod.hovering.primitive < 0)
        return;

    bsgfx_Primitive* hovering_primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, bsmod.hovering.primitive);

    bs_ivec2 coords = bsgfx_tileCoordinate(hovering_primitive, bsmod.hovering.tile_axis, bsmod.hovering.tile);

    bs_vec3 tile_position = bsgfx_tilePosition(hovering_primitive, bsmod.hovering.tile_axis, coords.x, coords.y);
    bs_vec4 rotation = bsgfx_tileRotation(bsmod.hovering.tile_axis);
    bs_vec3 euler_rotation = bsgfx_tileEulerRotation(bsmod.hovering.tile_axis);
    rotation = bs_qMulq(hovering_primitive->rotation, rotation);
    bs_mat4 matrix = bs_transform(tile_position, rotation, bs_v3V1(1.0));

    int subtype = bsgfx_primitiveSubtype(bsmod.dragging_id);
    bsgfx_instancePrimitive(subtype, matrix, 0, 0, $bsmod_light_blue()->id);

    if (bsmod_leftClickUpOnce()) {
        bsgfx_RawPrimitive* primitive = bsmod_add(BSGFX_TYPE_PRIMITIVE, &(bsgfx_RawPrimitive) {
            .position = tile_position,
            .rotation = euler_rotation,
            .scale = bs_v3V1(1),
            .type = bsmod.dragging_id,
            .guid = bs_guid(),
        });
        bsmod_saveType(BSGFX_TYPE_PRIMITIVE, "Created primitive");
    }
}



  /*==============================================================================
   * Rasterizing Icons
   *============================================================================*/

#include <float.h>

BSMODAPI void bsmod_rasterizePrimitiveIcons() {
#define WIDTH (70)
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
    bs_vec4 rotation = bs_qFromDegreesV3(15.0, 30.0, -15.0);

    for (int i = 0; i < BSGFX_PRIMITIVE_TYPE_COUNT; i++) {
        int subtype = bsgfx_primitiveSubtype(i);

        bs_Aabb aabb = { .min = bs_v3V1(-1.0), .max = bs_v3V1(1.0), };

        const float sphere_padding = 12.0;
        bs_mat4 transform = i == BSGFX_PRIMITIVE_TYPE_SPHERE ?
            bs_transform(BS_V3(render_size.x / 2, render_size.y / 2, 0.0), rotation, bs_v3V1(render_size.x / 2.0 - sphere_padding)) :
            bs_fitAabb(aabb, bs_v2Iv2(render_size), rotation);

        push_const.color = material->contract->color;
        push_const.material_texture_size = material->contract->image_binding;
        push_const.material_texture_id = material->contract->image;
        push_const.model = bs_m4x3(transform);

        instance = bsgfx_instancePrimitive(subtype, transform, 0, 0, 0);
        bsmod_rasterizeInstance(pipeline_hash, subtype, instance, material->category, material->name, render_size.x, render_size.y, sizeof(push_const), &push_const);
    }

    bsmod_endRasterize();
}