#include <ui/context/bsmod_ui_context.h>
#include <ui/light/bsmod_ui_light.h>
#include <ui/bsgfx_ui.h>
#include <ui/bsmod_ui.h>
#include <types/light/bsgfx_light.h>
#include <bsmod_type.h>
#include <bsmod_cache.h>

void bsmod_instanceContextMenu(bs_List* widgets, bs_vec3 position, bs_vec2 size) {
    bool hovering = bsgfx_instanceWidgets((bsgfx_Menu) {
        .position = position,
        .text_subtype = _bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS],
        .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head,
        .spacing = 4.0,
        .widgets = widgets->data,
        .widgets_count = widgets->count,
        .background_material_id_0 = $bsmod_grey_61()->id,
        .shadow_material_id = $bsmod_grey_30()->id,
        .untextured = {
            .dimensions = size,
            .auto_scale_width = true,
            .border_id = -1,
            // .has_shadow = true,
        },
        .blocked = false,
        .border_radius = BSMOD_DEFAULT_RADIUS,
    }, NULL, NULL);

    if (!_bsmod_.ui_blocked)
        _bsmod_.ui_blocked = hovering;
}

void bsmod_pushContextMenuButton(bs_List* widgets, bs_vec2 menu_size, bs_vec3 icon_offset, bsgfx_AtlasCache* icon, const char* name, int indent, bool(*action)(struct bsgfx_Widget*), bool expandable) {
    const int padding = BSMOD_CONTEXT_MENU_PADDING;
    const int align_height = BSMOD_CONTEXT_MENU_BUTTON_HEIGHT;

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .offset.x = padding,
        .type = BSGFX_WIDGET_BUTTON,
        .button = {
            .size = { menu_size.x - padding * 2, 20.0 },
        // .name = name,
            .tick = action,
            .material = $bsmod_grey_61()->id,
            .selected_material = $bsmod_grey_100()->id,
        },
        .align_height = align_height,
    });
    bsgfx_AtlasCache* expand = $BSMOD_ATLAS_UI_expand();

    bsgfx_Widget expand_widget = bsmod_iconWidget(expand, align_height, BS_V3(menu_size.x - padding * 2 - expand->size.x, 0.0, 0.0), 0);
    bsgfx_Widget icon_widget = bsmod_iconWidget(icon, align_height, bs_v3AddX(icon_offset, padding), BSGFX_WIDGET_ADVANCE_RIGHT);

    if (expandable)
        bs_pushBack(widgets, &expand_widget);
    bs_pushBack(widgets, &icon_widget);

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .offset.x = padding,
        .type = BSGFX_WIDGET_STRING,
        .string = {
            .value = name,
        },
        .align_height = align_height,
        .advance_flags = BSGFX_WIDGET_ADVANCE_DOWN,
    });

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .advance_flags = BSGFX_WIDGET_ADVANCE_RESET_X,
    });
}

void bsmod_instanceTileContextMenu(bs_vec3 position, bs_vec2 size) {
    static bs_List widgets = { .unit_size = sizeof(bsgfx_Widget), .increment = 16 };
    widgets.count = 0;

    const int indent_0 = 8;
    const int indent_1 = 32;

    bs_pushBack(&widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_STRING,
        .string = {
            .value = "Add",
        },
        .material_id = $bsmod_grey_100()->id,
        .offset = { indent_0, -indent_0, 0.0 },
        .advance_flags = BSGFX_WIDGET_ADVANCE_DOWN | BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Y,
    });

    bsgfx_Widget divider = bsmod_dividerWidget(size.x, indent_0);
    bs_pushBack(&widgets, &divider);

    bsmod_pushContextMenuButton(&widgets, size, BS_V3(0, -1, 0), $BSMOD_ATLAS_UI_light_add(), "Light", indent_1, bsmod_onAddLightTick, true);

    bsmod_instanceContextMenu(&widgets, position, size);
}
