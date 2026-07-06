#include <ui/bsmod_ui.h>
#include <ui/bsgfx_ui.h>
#include <bs_types.h>
#include <types/primitive/bsgfx_primitive.h>
#include <bsmod.h>
#include <bsmod_type.h>
#include <bsmod_cache.h>
#include <bsmod_rasterize.h>
#include <ui/grid/bsmod_ui_grid.h>
#include <types/prefab/bsgfx_prefab.h>



  /*==============================================================================
   * Dragging Material
   *============================================================================*/

void bsmod_onDragMaterial(bsmod_DraggingParams params) {
    if (bsmod.hovering.flags & BSGFX_ID_IS_PREFAB) {
        bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, bsmod.hovering.instance_id);
        bsgfx_RawPrefab* raw_prefab = bsgfx_getRaw(BSGFX_TYPE_PREFAB, bsmod.hovering.instance_id);
        if (bsmod_leftClickUpOnce()) {
            raw_prefab->material_hash = bsgfx_materialHash(bsmod.dragging_id);
            bsmod_saveType(BSGFX_TYPE_PREFAB, "Changed prefab material");
        }
    }

}



  /*==============================================================================
   * Side Menu Material UI
   *============================================================================*/

bs_String* bsmod_hex_input = NULL;
static bs_RGBA bsmod_material_color_input = { 255, 255, 255, 255 };

static bsgfx_Material* bsmod_selected_material;
static bs_String* bsmod_material_name = NULL;
static bs_vec4 bsmod_material_hsva = { 0, 0, 0, 1 };

static bsmod_GridClickParams bsmod_click_params;

static void bsgfx_onChangeColor(struct bsgfx_Widget* widget, bs_RGBA color) {
    bsmod_material_color_input = color;
    bsmod_hex_input = bs_stringF(bsmod_hex_input, "#%02X%02X%02X%02X", color.r, color.g, color.b, color.a);
    bsmod_selected_material->contract->color = bs_v4(color.r / 255.0, color.g / 255.0, color.b / 255.0, color.a / 255.0);
    if (widget)
        bsmod_selected_material->contract->hsva = bsmod_material_hsva;
}

void bsmod_onClickMaterialMenu(bsmod_GridClickParams params) {
    bs_Atlas* atlas = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_MATERIAL_ICONS)->atlas;

    char* name = atlas->unmapped[params.atlas_id].name;

    bsmod_selected_material = bsgfx_queryMaterial(name);
    bs_infoF("Selected material \"%s\"\n", name);

    bs_vec4 rgba_f = bs_v4MulV1(bsmod_selected_material->contract->color, 255.0);
    bs_RGBA rgba = bs_rgba(rgba_f.x, rgba_f.y, rgba_f.z, rgba_f.w);


    bsgfx_onChangeColor(NULL, rgba);

    bsmod_material_name = bs_string(bsmod_material_name, name, strlen(name));
    bsmod_material_hsva.xyz = bs_hsv(rgba);
    bsmod_material_hsva.w = bsmod_selected_material->contract->color.w;
    bsmod_selected_material->contract->hsva = bsmod_material_hsva;

    bsmod_click_params = params;
}

void bsmod_pushMaterialWidgets(bs_List* widgets, bs_vec2 background_size) {

    bs_vec3 offset = { 8 + 16, -8, 0 };
    offset.z += BSGFX_BACKGROUND_Z_COUNT;

    const int name_padding = 4;
    const int color_padding = 6;
    const int hue_width = 24;
    const int texture_dimensions = 128;
    const int color_dimensions = 215;
    const int channel_width = 72;

    static int select_position, select_size;

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_INPUT,
        //.name = "Name",
        .input = {
            .type = BSGFX_INPUT_STRING,
            .as_string = &bsmod_material_name,
            .dimensions = { background_size.x - offset.x, 16 },
            .outline_material_id = $bsmod_grey_40()->id,
            .background_outline_material_id = $bsmod_grey_30()->id,
            .select_position = &select_position,
            .select_size = &select_size,
        },
        .material_id = BSMOD_UI_BASE_TEXT_MATERIAL()->id,
        .offset = offset,
        .advance_flags = BSGFX_WIDGET_ADVANCE_DOWN,
    });

    bs_pushBack(widgets, &(bsgfx_Widget) {
       // .name = "Texture",
        .type = BSGFX_WIDGET_ICON,
        .offset = offset,
        .icon = {
            .type = BSGFX_ICON_MATERIAL,
            .scale = { texture_dimensions, texture_dimensions },
            .border_radius = BSMOD_DEFAULT_RADIUS,
            .outline_material_id = $bsmod_grey_30()->id,
            .material_id = bsmod_click_params.atlas_id,
        },
        .material_id = BSMOD_UI_BASE_TEXT_MATERIAL()->id,
        .advance_flags = BSGFX_WIDGET_ADVANCE_DOWN,
    });

    bs_pushBack(widgets, &(bsgfx_Widget) {
       // .name = "Color",
        .type = BSGFX_WIDGET_COLOR,
        .offset = offset,
        .color = {
            .dimensions = { color_dimensions, color_dimensions },
            .hsva = &bsmod_material_hsva,
            .hue_width = hue_width,
            .material_id = bsmod_selected_material->id,
            .background_material_id = $white_material()->id,
            .outline_material_id = $bsmod_grey_30()->id,
            .padding = color_padding,
            .border_radius = BSMOD_DEFAULT_RADIUS,
            .on_change_color = bsgfx_onChangeColor,
        },
        .material_id = BSMOD_UI_BASE_TEXT_MATERIAL()->id,
        .advance_flags = BSGFX_WIDGET_ADVANCE_DOWN,
        //.prefer_x = true,
    });

    //offset.x += color_padding+ hue_width * 2;
    //bsmod_pushInputWidget(widgets, &bsmod_red_input, offset, channel_width, name_padding, true, false, true, bsmod_onChangeRedInput, "");
    //bsmod_pushInputWidget(widgets, &bsmod_green_input, bs_v3AddY(offset, -(BSMOD_INPUT_HEIGHT + color_padding) * 1), channel_width, name_padding, true, false, true, bsmod_onChangeGreenInput, "");
    //bsmod_pushInputWidget(widgets, &bsmod_blue_input, bs_v3AddY(offset, -(BSMOD_INPUT_HEIGHT + color_padding) * 2), channel_width, name_padding, true, false, true, bsmod_onChangeBlueInput, "");
    //bsmod_pushInputWidget(widgets, &bsmod_alpha_input, bs_v3AddY(offset, -(BSMOD_INPUT_HEIGHT + color_padding) * 3), channel_width, name_padding, true, false, true, bsmod_onChangeAlphaInput, "");
    //bsmod_pushInputWidget(widgets, &bsmod_hex_input, bs_v3AddY(offset, -color_dimensions + BSMOD_INPUT_HEIGHT), channel_width, name_padding, true, true, false, bsmod_onChangeHexInput, "");
}



  /*==============================================================================
   * Rasterizing Icons
   *============================================================================*/

void bsmod_rasterizeMaterialIcons() {
#define WIDTH (74)
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
            bs_lookAt(bs_v3(0, 0, 1), bs_v3(0, 0, 0), bs_v3(0, 1, 0))
        )),
    };

    bsmod_beginRasterize(render_size, output_size);

    int sphere_subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_PRIMITIVE_SPHERE];

    bs_List* materials = bsgfx_materials();
    bs_mat4x3 transform = bs_m4x3(bs_transform(bs_v3(render_size.x / 2, render_size.y / 2, 0.0), BS_QUAT_IDENTITY, bs_v3V1(render_size.x / 2.0 - 4.0)));

    for (int i = 0; i < materials->count; i++) {
        bsgfx_Material* material = bs_fetchUnit(materials, i);
        push_const.color = material->contract->color;
        push_const.material_texture_size = material->contract->image_binding;
        push_const.material_texture_id = material->contract->image;
        push_const.model = transform;

        int instance = bsgfx_instancePrimitive(sphere_subtype, BS_MAT4_IDENTITY, 0, 0, 0);
        bsmod_rasterizeInstance(pipeline_hash, sphere_subtype, instance, material->category, material->name, render_size.x, render_size.y, sizeof(push_const), &push_const);
    }

    bsmod_endRasterize();
}