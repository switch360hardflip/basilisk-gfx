#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <bsgfx.h>
#include <bs_json.h>
#include <bs_window.h>
#include <bsmod_compile.h>
#include <bsmod_cache.h>
#include <_bsmod_.h>

#include <ui/bsgfx_ui.h>
#include <ui/grid/bsmod_ui_grid.h>
#include <ui/side/bsmod_ui_side.h>
#include <ui/bsmod_ui.h>
#include <ui/material/bsmod_ui_material.h>
#include <ui/primitive/bsmod_ui_primitive.h>
#include <ui/tile/bsmod_ui_tile.h>
#include <ui/prefab/bsmod_ui_prefab.h>
#include <ui/type/bsmod_ui_type.h>

#include <types/prefab/bsgfx_prefab.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/tile/bsgfx_tile.h>

static int _bsmod_active_side_menu_tab;

static void bsmod_pushInstanceWidgets(bs_List* widgets, bs_vec2 background_size) {
    switch (_bsmod_.selected_type) {
    case BSGFX_TYPE_PREFAB: bsmod_pushPrefabMenuWidgets(widgets, background_size); break;
    case BSGFX_TYPE_TILE: bsmod_pushTileMenuWidgets(widgets, background_size); break;
    default:

        break;
    }
}

bsmod_SideMenuTab _bsmod_side_menu_tabs[BSMOD_TABS_COUNT] = {
    [BSMOD_TAB_INSTANCE] = {.name = "Instance", .cache = $BSMOD_ATLAS_UI_instance, .push_widgets = bsmod_pushInstanceWidgets , },
    [BSMOD_TAB_MATERIAL] = {.name = "Material", .cache = $BSMOD_ATLAS_UI_material, .push_widgets = bsmod_pushMaterialWidgets, .on_click = bsmod_onClickMaterialMenu },
    [BSMOD_TAB_SETTINGS] = {.name = "Settings", .cache = $BSMOD_ATLAS_UI_instance, },
};

void bsmod_setSideMenuTab(int id, bsmod_GridClickParams params) {
    assert(id >= 0 && id < BSMOD_TABS_COUNT);

    _bsmod_active_side_menu_tab = id;
    if (_bsmod_side_menu_tabs[id].on_click)
        _bsmod_side_menu_tabs[id].on_click(params);
}

void bsmod_instanceSideMenu(bs_vec3 position, bs_vec2 dimensions) {
    const int width = 64;
    static int scroll;
    static bs_List tabs = {.unit_size = sizeof(bsgfx_MenuTab), .increment = 16 };
    static bs_List widgets = { .unit_size = sizeof(bsgfx_Widget), .increment = 16 };

    tabs.count = 0;
    widgets.count = 0;

    const int padding = 8;
    bs_vec2 background_size = { dimensions.x - padding * 2, dimensions.y - padding * 2 };

    bs_pushBack(&widgets, &(bsgfx_Widget) {
        .offset = { 0, -padding, 0 },
        .advance_flags = BSGFX_WIDGET_ADVANCE_APPLY_OFFSET,
    });

    bs_pushBack(&widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_BACKGROUND,
        .background = {
            .border_radius = BSMOD_DEFAULT_RADIUS,
            .size = background_size,
            .material_id = $bsmod_grey_61()->id,
            .outline_material_id = $bsmod_grey_30()->id,
            .subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI_COLOR],
        },
        .offset = { padding, 0.0, 0 },
    });

    background_size.x -= padding;
    for (int i = 0; i < (sizeof(_bsmod_side_menu_tabs) / sizeof(*_bsmod_side_menu_tabs)); i++) {
        bsmod_SideMenuTab* tab_info = _bsmod_side_menu_tabs + i;
        bsgfx_MenuTab* tab = bs_pushBack(&tabs, &(bsgfx_MenuTab) {.icon_cache = tab_info->cache(), .name = tab_info->name, });
        tab->icon_material_id = $white_material()->id;

        if (_bsmod_active_side_menu_tab == i) {
            tab->material_id = $bsmod_grey_120()->id;
            tab->shadow_material_id = $bsmod_grey_61()->id;
            tab->outline_material_id = $bsmod_grey_148()->id;

            if (tab_info->push_widgets)
                tab_info->push_widgets(&widgets, background_size);
        }
        else {
            tab->material_id = $context_menu_title_bar_material()->id;
            tab->outline_material_id = $bsmod_grey_148()->id;
            tab->shadow_material_id = $bsmod_grey_61()->id;
        }
    }

    position.z += 10;

    bool hovering = bsgfx_instanceWidgets((bsgfx_Menu) {
        .position = position,
        .text_subtype = _bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS],
        .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head,
        .spacing = 8.0,
        .widgets = widgets.data,
        .widgets_count = widgets.count,
        .background_material_id_0 = $context_menu_title_bar_transparent_material()->id,
        .background_material_id_1 = $context_menu_title_bar_material()->id,
        .untextured = {
            .dimensions = dimensions,
            .auto_scale_width = true,
            .border_id = -1,
            // .has_shadow = true,
        },
        .blocked = false,
        .border_radius = BSMOD_DEFAULT_RADIUS,
        .shadow_material_id = $bsmod_grey_61()->id,
        .outline_material_id = $bsmod_grey_148()->id,
    }, NULL,
    // &(bsgfx_TitleBar) {
    //    .name = "",
    //    .border_radius = BSMOD_DEFAULT_RADIUS,
    //    .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16),
    //    .material_id = $bsmod_grey_120()->id,
    //    .button_hovering_material_id = $bsmod_red()->id,
    //    .button_icon_material_id = $bsmod_grey_120()->id,
    //    .button_unavailable_material_id = $bsmod_grey_148()->id,
    //    .button_material_id = $bsmod_grey_61()->id,
    //    .button_shadow_material_id = $bsmod_grey_112()->id,
    //}, 
    &(bsgfx_MenuTabBar) {
        .tabs_count = tabs.count,
        .tabs = tabs.data,
        .active_tab = &_bsmod_active_side_menu_tab,
        .material_id = $bsmod_grey_61()->id,
        .outline_material_id = $bsmod_grey_61()->id,
        .shadow_material_id = $bsmod_grey_30()->id,
        .height = BSMOD_TAB_BAR_HEIGHT,
    });

    if (!_bsmod_.ui_blocked)
        _bsmod_.ui_blocked = hovering;
}
