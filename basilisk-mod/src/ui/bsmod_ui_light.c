#include <ui/bsgfx_ui.h>
#include <ui/bsmod_ui.h>
#include <ui/context/bsmod_ui_context.h>
#include <bsmod.h>
#include <bsmod_type.h>
#include <types/light/bsgfx_light.h>
#include <types/tile/bsgfx_tile.h>
#include <bsmod_cache.h>



  /*==============================================================================
   * Billboards
   *============================================================================*/

void bsmod_instanceLightBillboards() {
    bs_vec2 resolution = BS_IV2_TO_V2(bs_resolution());
    const bs_mat4 identity = BS_MAT4_IDENTITY;

    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_LIGHT); i++) {
        bsgfx_Light* light = bsgfx_get(BSGFX_TYPE_LIGHT, i);

        bs_mat4x3 m = bsgfx_matrix(light->position, BS_V3(1.0, 1.0, 0.0));
        bsgfx_AtlasCache* cache = NULL;

        switch (light->type) {
        case BSGFX_LIGHT_TYPE_POINT:
            cache = $BSMOD_ATLAS_UI_light();
            break;
        case BSGFX_LIGHT_TYPE_SUN:
            cache = $BSMOD_ATLAS_UI_sun();
            break;
        default: continue;
        }

        bsgfx_instanceQuad(_bsmod_subtypes[BSMOD_SUBTYPE_BILLBOARD], m, cache->coords, 0, 0, $white_material()->id);

        const float length = 100.0;
        bsgfx_lineScreenInstance(light->position, bs_v3Add(light->position, bs_v3MulS(light->direction, length)), BS_BLACK);

        bs_vec2 p = bsgfx_worldToScreen(light->position, poser()->camera.result, resolution);

        bs_vec4 clip = bs_m4MulV4(poser()->camera.result, bs_v4V3(light->position, 1.0f));

        float pixels_per_unit = poser()->camera.proj.a[1][1] * resolution.y * 0.5f;
        float scaled_size = pixels_per_unit / clip.w;

        p.x -= scaled_size * 0.5;
        p.y -= scaled_size * 0.5;

       // bsgfx_AtlasCache* white = $BSMOD_ATLAS_UI_white();
       // bs_mat4x3 m2 = bsgfx_matrix(bs_v3V2(p, 50.0f), BS_V3(scaled_size, scaled_size, 0.0f));
       // bsgfx_instanceQuad(bsgfx_subtypes()[BSGFX_SUBTYPE_UI], m2, white->coords, 0, 0, $white_material()->id);

        bool hovering = bs_rectangleVsPoint(p, BS_V2(scaled_size, scaled_size), bs_cursorPosition());
        if (hovering) {
            bsmod.hovering.billboard = true;

            if (bsmod_leftClickOnce()) {
                if (!bsmod_keyDown(BS_KEY_ALT))
                    bsmod_deselectAll();

                bsmod_select(&bsmod.selected_ids, BSGFX_TYPE_LIGHT, i);
            }
        }
    }
}



  /*==============================================================================
   * Context Menu
   *============================================================================*/

static inline void bsmod_addLight(bsgfx_LightType type) {
    assert(bsmod.selected_tiles.count == 1);

    int tile_id = *(int*)bs_fetchUnit(&bsmod.selected_tiles, 0);
    int primitive_id = bsgfx_queryTilePrimitive(tile_id);
    bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, primitive_id);

    int axis = bsgfx_tileAxis(primitive, tile_id);
    bs_ivec2 coords = bsgfx_tileCoordinate(primitive, axis, tile_id);

    bsmod_add(BSGFX_TYPE_LIGHT, &(bsgfx_RawLight) {
        .type = type,
        .position = bsgfx_tilePosition(primitive, axis, coords.x, coords.y),
        .rotation = { 0.0 }
        //.direction = { 0.0, -1.0, 0.0 },
    });
}

static bool bsmod_onAddPointTick(bsgfx_ButtonParams params) {
    if (params.hovering && bsmod_leftClickOnce())
        bs_warnF("Point light is not implemented\n");

    return params.hovering;
}

static bool bsmod_onAddSunTick(bsgfx_ButtonParams params) {
    if (params.hovering && bsmod_leftClickOnce())
        bsmod_addLight(BSGFX_LIGHT_TYPE_SUN);

    return params.hovering;
}

bool bsmod_onAddLightTick(bsgfx_ButtonParams params) {
    static bool was_hovering;

    bs_vec3 position = params.widget_position;
    position.y += BSMOD_CONTEXT_MENU_BUTTON_HEIGHT;
    position.x += params.menu->untextured.dimensions.x;
    position.x -= BSMOD_CONTEXT_MENU_PADDING - 1;

    bs_vec2 size = BS_V2(125.0, 175.0);

    if (!params.hovering && was_hovering) {
        if (bs_rectangleVsPoint(BS_V2(position.x - (BSMOD_CONTEXT_MENU_PADDING + 2), position.y - size.y), size, bs_cursorPosition())) {
            params.hovering = true;
        }
    }

    if (params.hovering) {
        static bs_List widgets = { .unit_size = sizeof(bsgfx_Widget), .increment = 16};
        widgets.count = 0;

        const int indent = 4;
        //bsgfx_Widget sun_widget = bsmod_iconWidget(icon, align_height, bs_v3AddX(icon_offset, padding), BSGFX_WIDGET_ADVANCE_RIGHT);
        bsmod_pushContextMenuButton(&widgets, size, BS_V3(0, -1, 0), $BSMOD_ATLAS_UI_light_add(), "Point", 0, bsmod_onAddPointTick, false);
        bsmod_pushContextMenuButton(&widgets, size, BS_V3(0, 0, 0), $BSMOD_ATLAS_UI_sun_add(), "Sun", 0, bsmod_onAddSunTick, false);

        bsmod_instanceContextMenu(&widgets, position, size);
    }

    was_hovering = params.hovering;
    return params.hovering;
}
