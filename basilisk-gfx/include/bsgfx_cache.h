#ifndef BSGFX_PREQUERY_H
#define BSGFX_PREQUERY_H

#include <bs_shaders.h>
#include <bs_models.h>
#include <bs_objects.h>
#include <bs_mem.h>
#include <bsgfx_material.h>
#include <bsgfx_instance.h>
#include <bsgfx.h>

#define BSGFX_CACHE_SHADER(path, type)                                                          \
    inline bs_Shader* $##path##() {                                                             \
        static bs_Object* object = NULL;                                                        \
        if (!object) {                                                                          \
            bs_shader(BSGFX_PACKAGE, "shaders/" #path "." #type, 0, &object);                   \
        }                                                                                       \
        return object->shader;                                                                  \
    }

#define BSGFX_CACHE_VS(path) BSGFX_CACHE_SHADER(path, vert)
#define BSGFX_CACHE_FS(path) BSGFX_CACHE_SHADER(path, frag)
#define BSGFX_CACHE_CS(path) BSGFX_CACHE_SHADER(path, comp)
#define BSGFX_CACHE_RGEN(path) BSGFX_CACHE_SHADER(path, rgen)
#define BSGFX_CACHE_RMISS(path) BSGFX_CACHE_SHADER(path, rmiss)

#define BSGFX_CACHE_MODEL(path, flags)                                                          \
    inline bs_Model* $##path##() {                                                              \
        static bs_Model* model = NULL;                                                          \
        const char* p = #path + 1;                                                              \
        if (!model)                                                                             \
            model = bs_model(BSGFX_PACKAGE, "models/" #path, flags)->model;                     \
        return model;                                                                           \
    }

static inline bs_vec4 bsgfx_convertColor(bs_RGBA color) {
    return BS_V4((float)color.r / 255.0, (float)color.g / 255.0, (float)color.b / 255.0, (float)color.a / 255.0);
}

#define BSGFX_CACHE_COLOR_MATERIAL(name, _color)                                    \
    inline bsgfx_Material* $##name##() {                                            \
        static int id = -1;                                                         \
        if (id == -1) { \
            id = bsgfx_material(#name)->id;                                         \
            bsgfx_Material* material = bs_fetchUnit(bsgfx_materials(), id);             \
        material->category = BSGFX_CATEGORY;                                        \
        material->contract->color = bsgfx_convertColor(_color);                     \
        }                                                               \
        bsgfx_Material* material = bs_fetchUnit(bsgfx_materials(), id);             \
        return material;                                                            \
    }

typedef struct {
    const char* name;
    int id;
    bs_vec4 coords;
    bs_vec2 size;
} bsgfx_AtlasCache;
#define BSGFX_CACHE_ATLAS_QUERY(source_id, atlas_id, n)                             \
    inline bsgfx_AtlasCache* $##atlas_id##_##n##() {                                \
        static bsgfx_AtlasCache cache = {.id = -1};                                 \
        if (cache.id == -1) {                                                       \
            bs_Atlas* atlas = bs_fetch(source_id, atlas_id)->head;                  \
            cache.name = #n;                                                        \
            cache.id = bs_queryAtlas(atlas, #n);                                    \
            cache.coords = bs_atlasCoordinates(atlas, cache.id, 0);                 \
            cache.size = bs_atlasSize(atlas, cache.id);                             \
        }                                                                           \
        return &cache;                                                              \
    }



  /*==============================================================================
   * Atlas Cache
   *============================================================================*/

BSGFX_CACHE_ATLAS_QUERY(BSGFX_ATLASES, BSGFX_ATLAS_ANY, white)
BSGFX_CACHE_ATLAS_QUERY(BSGFX_ATLASES, BSGFX_ATLAS_ANY, ui_copy)
BSGFX_CACHE_ATLAS_QUERY(BSGFX_ATLASES, BSGFX_ATLAS_ANY, ui_increment)



  /*==============================================================================
   * Shader Cache
   *============================================================================*/

#define BSGFX_PACKAGE bsgfx_package()
BSGFX_CACHE_VS(vs_bsgfx_mesh_instanced_2d)
BSGFX_CACHE_VS(vs_bsgfx_mesh_instanced)
BSGFX_CACHE_VS(vs_bsgfx_mesh_static_instanced)
BSGFX_CACHE_VS(vs_bsgfx_bone_instanced)
BSGFX_CACHE_VS(vs_bsgfx_bone_instanced_2d)
BSGFX_CACHE_VS(vs_bsgfx_quad_instanced)
BSGFX_CACHE_VS(vs_bsgfx_quad_rounded_instanced)
BSGFX_CACHE_VS(vs_bsgfx_tile_static)
BSGFX_CACHE_VS(vs_bsgfx_color_percentage)
BSGFX_CACHE_VS(vs_bsgfx_foliage)
BSGFX_CACHE_VS(vs_bsgfx_line_instanced)
BSGFX_CACHE_VS(vs_bsgfx_point_instanced)
BSGFX_CACHE_VS(vs_bsgfx_volume)
BSGFX_CACHE_VS(vs_bsgfx_volume_isolated)
BSGFX_CACHE_VS(vs_bsgfx_volume_mesh_instanced)
BSGFX_CACHE_VS(vs_bsgfx_mesh_color)

BSGFX_CACHE_FS(fs_bsgfx_lo_res_ui_post_0)
BSGFX_CACHE_FS(fs_bsgfx_pixelation)
BSGFX_CACHE_FS(fs_bsgfx_hilight)
BSGFX_CACHE_FS(fs_bsgfx_model)
BSGFX_CACHE_FS(fs_bsgfx_material_texture)
BSGFX_CACHE_FS(fs_bsgfx_color)
BSGFX_CACHE_FS(fs_bsgfx_color_depthless)
BSGFX_CACHE_FS(fs_bsgfx_color_percentage)
BSGFX_CACHE_FS(fs_bsgfx_error)
BSGFX_CACHE_FS(fs_bsgfx_alpha)
BSGFX_CACHE_FS(fs_bsgfx_hue)
BSGFX_CACHE_FS(fs_bsgfx_color_picker)
BSGFX_CACHE_FS(fs_bsgfx_level_thumbnail)
BSGFX_CACHE_FS(fs_bsgfx_atlas)
BSGFX_CACHE_FS(fs_bsgfx_atlas_rounded)
BSGFX_CACHE_FS(fs_bsgfx_minimap)
BSGFX_CACHE_FS(fs_bsgfx_tile)
BSGFX_CACHE_FS(fs_bsgfx_ui)
BSGFX_CACHE_FS(fs_bsgfx_ui_color)
BSGFX_CACHE_FS(fs_bsgfx_dither)
BSGFX_CACHE_FS(fs_bsgfx_tile_selected)
BSGFX_CACHE_FS(fs_bsgfx_tile_hi)
BSGFX_CACHE_FS(fs_bsgfx_tile_screen)
BSGFX_CACHE_FS(fs_bsgfx_volume)
BSGFX_CACHE_FS(fs_bsgfx_volume_result)
BSGFX_CACHE_FS(fs_bsgfx_volume_screen)
BSGFX_CACHE_FS(fs_bsgfx_volume_texture)
BSGFX_CACHE_FS(fs_bsgfx_font_temple)
BSGFX_CACHE_FS(fs_bsgfx_font_consolas)
BSGFX_CACHE_FS(fs_bsgfx_font_arial)
BSGFX_CACHE_FS(fs_bsgfx_font_small)
BSGFX_CACHE_FS(fs_bsgfx_font_standard)
BSGFX_CACHE_FS(fs_bsgfx_256_hi_res)
BSGFX_CACHE_FS(fs_bsgfx_edge_gradient)

BSGFX_CACHE_CS(cs_bsgfx_mesh_volume)
BSGFX_CACHE_CS(cs_bsgfx_mesh_type_volume)
BSGFX_CACHE_CS(cs_bsgfx_bone_volume)
BSGFX_CACHE_CS(cs_bsgfx_textured_volume)

//BSGFX_CACHE_RMISS(main)
//BSGFX_CACHE_RGEN(main)



  /*==============================================================================
   * Material Cache
   *============================================================================*/


#define BSGFX_PACKAGE bsgfx_package()
#define BSGFX_CATEGORY BSGFX_MATERIAL_CATEGORY_NONE

BSGFX_CACHE_COLOR_MATERIAL(menu_background, bs_rgba(112, 112, 112, 248))
BSGFX_CACHE_COLOR_MATERIAL(menu_border, bs_rgba(40, 40, 40, 248))
BSGFX_CACHE_COLOR_MATERIAL(white_material, BS_WHITE)
BSGFX_CACHE_COLOR_MATERIAL(blank_material, BS_BLANK)
BSGFX_CACHE_COLOR_MATERIAL(black_material, BS_BLACK)
BSGFX_CACHE_COLOR_MATERIAL(red_material, BS_RED)
BSGFX_CACHE_COLOR_MATERIAL(cyan_material, BS_CYAN)
BSGFX_CACHE_COLOR_MATERIAL(yellow_material, BS_YELLOW)
BSGFX_CACHE_COLOR_MATERIAL(magenta_material, BS_MAGENTA)
BSGFX_CACHE_COLOR_MATERIAL(green_material, BS_GREEN)
BSGFX_CACHE_COLOR_MATERIAL(blue_material, BS_BLUE)
BSGFX_CACHE_COLOR_MATERIAL(gray_material, bs_rgba(112, 112, 112, 255))
BSGFX_CACHE_COLOR_MATERIAL(light_gray_material, bs_rgba(155, 155, 155, 255))
BSGFX_CACHE_COLOR_MATERIAL(ui_tab_material, bs_rgba(197, 88, 98, 255))
BSGFX_CACHE_COLOR_MATERIAL(ui_green_material, bs_rgba(5, 170, 10, 255))
BSGFX_CACHE_COLOR_MATERIAL(ui_light_green_material, bs_rgba(10, 200, 20, 255))
BSGFX_CACHE_COLOR_MATERIAL(half_transparent_material, bs_rgba(200, 200, 200, 255))
BSGFX_CACHE_COLOR_MATERIAL(context_menu_background_material, bs_rgba(106, 112, 115, 245))

#define context_menu_title_bar_material_base(alpha) bs_rgba(90, 90, 90, alpha)
BSGFX_CACHE_COLOR_MATERIAL(context_menu_title_bar_material, context_menu_title_bar_material_base(255))
BSGFX_CACHE_COLOR_MATERIAL(context_menu_title_bar_transparent_material, context_menu_title_bar_material_base(0))

BSGFX_CACHE_COLOR_MATERIAL(context_menu_grey_text_material, bs_rgba(60, 60, 60, 255))

BSGFX_CACHE_COLOR_MATERIAL(bsmod_blue_border, bs_rgba(75, 167, 114, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_blue, bs_rgba(135, 232, 57, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_red, bs_rgba(230, 73, 58, 255))

#define BSGFX_CATEGORY BSGFX_MATERIAL_CATEGORY_UI_COLOR_SCHEME

BSGFX_CACHE_COLOR_MATERIAL(bsmod_light_blue, bs_rgba(175, 204, 222, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_30, bs_rgba(20, 20, 20, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_40, bs_rgba(40, 40, 40, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_61, bs_rgba(60, 60, 60, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_100, bs_rgba(100, 100, 100, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_112, bs_rgba(112, 112, 112, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_120, bs_rgba(120, 120, 120, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_138, bs_rgba(138, 138, 138, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_148, bs_rgba(146, 148, 148, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_158, bs_rgba(158, 158, 158, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_182, bs_rgba(182, 182, 182, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_200, bs_rgba(200, 200, 200, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_226, bs_rgba(226, 226, 226, 255))
BSGFX_CACHE_COLOR_MATERIAL(bsmod_grey_230, bs_rgba(230, 230, 230, 255))
#undef BSGFX_CATEGORY
#undef BSGFX_PACKAGE

#endif