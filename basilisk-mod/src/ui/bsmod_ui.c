#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <bsgfx.h>
#include <bs_json.h>
#include <bs_window.h>
#include <bsmod_compile.h>
#include <bsmod_type.h>
#include <bsmod_cache.h>
#include <bsmod.h>

#include <ui/bsgfx_ui.h>
#include <ui/bsmod_ui.h>
#include <ui/primitive/bsmod_ui_primitive.h>
#include <ui/material/bsmod_ui_material.h>
#include <ui/tile/bsmod_ui_tile.h>
#include <ui/prefab/bsmod_ui_prefab.h>
#include <ui/light/bsmod_ui_light.h>
#include <ui/grid/bsmod_ui_grid.h>
#include <ui/side/bsmod_ui_side.h>
#include <ui/context/bsmod_ui_context.h>

#include <types/prefab/bsgfx_prefab.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/tile/bsgfx_tile.h>
#include <types/light/bsgfx_light.h>
#include <bsgfx_pipeline.h>

void bsmod_pushInputWidget(
    bs_List* widgets,
    void* value,
    bs_vec3 offset, 
    float width, 
    int name_padding, 
    bool skip_advance, 
    bool is_last, 
    bool has_buttons,
    const char* name) 
{
    static int select_position, select_size;
    if (has_buttons) {
        const int button_padding = 2;
        bsgfx_AtlasCache* increment = $BSMOD_ATLAS_UI_increment();
        bs_pushBack(widgets, &(bsgfx_Widget) {
            //.name = "",
            .type = BSGFX_WIDGET_ICON,
            .offset = bs_v3Add(offset, bs_v3(width - increment->size.x - button_padding, -button_padding, 10.0)),
            .icon = {
                .atlas = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_UI)->atlas,
                .atlas_subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
                .type = BSGFX_ICON_ATLAS,
                .name = "increment",
                .material_id = $bsmod_grey_120()->id,
                //.placement = BSGFX_ICON_PLACE_BELOW
                //.hover = bsmod_onHoverDirectoryWidget,
            },
            .advance_flags = BSGFX_WIDGET_ADVANCE_RIGHT,
            .material_id = $bsmod_grey_120()->id,
        });

        bs_pushBack(widgets, &(bsgfx_Widget) {
           // .name = "",
            .type = BSGFX_WIDGET_ICON,
            .offset = bs_v3Add(offset, bs_v3(width - increment->size.x - button_padding, -BSMOD_INPUT_HEIGHT + increment->size.y + button_padding, 10.0)),
            .icon = {
                .atlas = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_UI)->atlas,
                .atlas_subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
                .type = BSGFX_ICON_ATLAS,
                .name = "increment",
                .material_id = $bsmod_grey_120()->id,
                //.placement = BSGFX_ICON_PLACE_BELOW,
                .flipped = true,
                //.hover = bsmod_onHoverDirectoryWidget,
            },
            .advance_flags = BSGFX_WIDGET_ADVANCE_RIGHT,
            .material_id = $bsmod_grey_120()->id,
        });
    }

    bs_pushBack(widgets, &(bsgfx_Widget) {
       // .name = name,
        .type = BSGFX_WIDGET_INPUT,
        .offset = offset,
        .input = {
            .dimensions = { width, BSMOD_INPUT_HEIGHT },
            .as_void = value,
            .placeholder_text = "",
            .placeholder_text_material_id = $bsmod_grey_120()->id,
            .outline_material_id = $bsmod_grey_40()->id,
            .background_outline_material_id = $bsmod_grey_30()->id,
            .select_position = &select_position,
            .select_size = &select_size,
        },
        .advance_flags = is_last ? BSGFX_WIDGET_ADVANCE_RESET_X : BSGFX_WIDGET_ADVANCE_RIGHT,
        .material_id = $bsmod_grey_120()->id,
    });
}

void bsmod_pushVecNWidget(bs_List* widgets, const char* name, bs_vec3 offset, float width, float* v, int n) {
    assert(n <= 4);

    const char* xyzw[] = { "x", "y", "z", "w" };

    const float align_height = 16;

    if (name) {
        bs_pushBack(widgets, &(bsgfx_Widget) {
            .type = BSGFX_WIDGET_STRING,
            .string.value = name,
            .offset = offset,
            .advance_flags = 0,
            .align_height = align_height,
        });
        offset.x += 96.0;
    }

    for (int i = 0; i < n; i++) {
        bs_pushBack(widgets, &(bsgfx_Widget) {
            .type = BSGFX_WIDGET_STRING,
            .offset = offset,
            .string = {
                .value = xyzw[i]
            },
            .advance_flags = BSGFX_WIDGET_ADVANCE_RIGHT,
            .align_height = align_height,
        });

        bs_pushBack(widgets, &(bsgfx_Widget) {
            .type = BSGFX_WIDGET_INPUT,
            .input = {
                .type = BSGFX_INPUT_FLOAT,
                .outline_material_id = $bsmod_grey_40()->id,
                .background_outline_material_id = $bsmod_grey_30()->id,
                .dimensions = { width, 16.0 },
                .as_float = v + i,
            },
            .offset = offset,
            .advance_flags = i == (n - 1) ? BSGFX_WIDGET_ADVANCE_RESET_X | BSGFX_WIDGET_ADVANCE_DOWN : BSGFX_WIDGET_ADVANCE_RIGHT,
            .align_height = align_height,
        });
        offset.x += 4.0;
    }
}

#define BSMOD_DIRECTORY_BACKGROUND_PADDING 8
#define BSMOD_DIRECTORY_BACKGROUND_DIMENSIONS ((bs_vec2) { 256, 512 })

void bsmod_instanceBackgroundMenu(bs_vec3 position, bs_vec2 dimensions) {
    const int width = 64;
    static int scroll;
    static bs_List tabs = { .unit_size = sizeof(bsgfx_MenuTab), .increment = 16 };
    static int active_tab;
    if (tabs.count == 0) {
        int material = $bsmod_grey_30()->id;
    }

    for (int i = 0; i < tabs.count; i++) {
        bsgfx_MenuTab* tab = bs_fetchUnit(&tabs, i);
        if (active_tab == i) {
            tab->material_id = $bsmod_grey_120()->id;
            tab->shadow_material_id = $bsmod_grey_61()->id;
            tab->outline_material_id = $bsmod_grey_148()->id;
        }
        else {

            tab->material_id = $context_menu_title_bar_material()->id;
            tab->outline_material_id = $bsmod_grey_148()->id;
            tab->shadow_material_id = $bsmod_grey_61()->id;
        }
    }

    const int padding = 24.0;

    position.z -= 13.0;
    position.x -= padding;
    position.y += padding;
    position.y += BSMOD_TAB_BAR_HEIGHT;
    dimensions.x += BSMOD_GRID_SIDE_MENU_SPACE;
    dimensions.x += padding * 2;
    dimensions.y += padding * 2;
    dimensions.y += BSMOD_TAB_BAR_HEIGHT;

    bool hovering = bsgfx_instanceWidgets((bsgfx_Menu) {
        .position = position,
        .text_subtype = _bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS],
        .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head,
        .spacing = 4.0,
        .widgets = NULL,
        .widgets_count = 0,
        .background_material_id_0 = $white_material()->id,
        .untextured = {
            .dimensions = dimensions,
            .auto_scale_width = true,
            .border_id = -1,
            // .has_shadow = true,
        },
        .blocked = false,
        .border_radius = BSMOD_DEFAULT_RADIUS,
        //.shadow_material_id = $bsmod_grey_61()->id,
        //.outline_material_id = $bsmod_grey_148()->id,
    }, & (bsgfx_TitleBar) {
        .name = "Window",
        .border_radius = BSMOD_DEFAULT_RADIUS,
        .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head,
        .material_id = $white_material()->id,
        .button_hovering_material_id = $bsmod_red()->id,
        .button_icon_material_id = $bsmod_grey_120()->id,
        .button_unavailable_material_id = $bsmod_grey_148()->id,
        .button_material_id = $bsmod_grey_61()->id,
        .button_shadow_material_id = $bsmod_grey_112()->id,
    }, NULL);

    if (!bsmod.ui_blocked)
        bsmod.ui_blocked = hovering;
}

static void bsmod_instanceDraggingIcon() {
    bsmod_DraggingParams params = { 0 };
    bs_vec2 cursor = bs_windowCursorPosition();

    bs_Atlas* atlas_ui = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_UI)->atlas;
    bsgfx_AtlasCache* cache = $BSMOD_ATLAS_UI_prohibition();

    bsgfx_instanceQuad(
        bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
        bsgfx_matrix(
            bs_v3(cursor.x, cursor.y, 101.0),
            bs_v3(cache->size.x, cache->size.y, 0.0)
        ),
        cache->coords,
        0, 0, 0);

    switch (bsmod.dragging_object_id) {
    case BSMOD_ATLAS_MATERIAL_ICONS: bsmod_onDragMaterial(params); break;
    case BSMOD_ATLAS_PREFAB_ICONS: bsmod_onDragPrefab(params); break;
    case BSMOD_ATLAS_PRIMITIVE_ICONS: bsmod_onDragPrimitive(params); break;
    case BSGFX_IMAGE_TILE:  bsmod_onDragTile(params); break;
    default:
        return;
    }

    if (bsmod_leftClickUpOnce()) {
        bs_infoF("Drop\n");
        bsmod.dragging_id = -1;
        bsmod.dragging_object_id = 0;
        bsmod.dragging_subtype = -1;
    }
}

void bsmod_renderBillboards() {
    bs_beginComment("Billboards");
    struct {
        bs_mat4 camera;
        bs_mat4 view;

    } push_const = {
        .camera = poser()->camera.result,
        .view = poser()->camera.view,
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsmod_billboard(),
            $fs_bsmod_billboard(),
        },
        .skip_depth_test = true,
        BSGFX_TRANSPARENT_OPTIONS,

    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(_bsmod_subtypes[BSMOD_SUBTYPE_BILLBOARD], pipeline);
    bs_endComment();
}

static void bsmod_instanceBillboards() {
    bsmod.hovering.billboard = false;
    bsmod_instanceLightBillboards();
}

void bsmod_instanceUI() {
    bsmod.ui_blocked = false;
    static bool right_clicked;
    static bs_vec2 position;

    if (bsmod_rightClickUpOnce()) {
        right_clicked = true;
        position = bs_windowCursorPosition();
    }

    if (bsmod_leftClickUpOnce() || bsmod_middleClickOnce())
        right_clicked = false;

    if (right_clicked) {
        bs_vec3 p = bs_v3(position.x, position.y, 20.0);
        bs_vec2 s = { 200.0, 300.0 };

        float d = p.y - s.y;

        if (d < 0.0)
            p.y -= d;

        if (bsmod.selected_type == BSGFX_TYPE_TILE && bsmod.selected_tiles.count == 1)
            bsmod_instanceTileContextMenu(p, s);
    }

    if (bsmod_keyDown(BS_KEY_ALT) && !bsmod_keyDown(BS_KEY_LEFT_CONTROL)) {
        if (bsmod_keyDownOnce(BS_KEY_1)) {
            bsmod_instance_grid_menu = true;

        }

        for (int i = BS_KEY_2; i < BS_KEY_9; i++) {
            if (bsmod_keyDownOnce(i)) {
                bsmod_instance_grid_menu = false;
                bs_disableUserInputs(false);
            }
        }
    }

    if (bsmod.dragging_subtype >= 0)
        bsmod_instance_grid_menu = false;

    if (bsmod_instance_grid_menu) {
        bs_disableUserInputs(true);

        bs_vec2 dimensions = { 1600.0, 900.0 };
        bs_ivec2 resolution = bs_resolution();
        bs_vec3 center = {
            (int)((resolution.x - dimensions.x) / 2.0),
            (int)((resolution.y + dimensions.y) / 2.0),
            25.0
        };

      //  bsmod_instanceBackgroundMenu(center, dimensions);

        const int side_menu_width = BSGFX_SIDE_MENU_WIDTH;
        dimensions.x -= side_menu_width;
        bsmod_instanceGridMenu(center, dimensions);

        center.x += dimensions.x;
        center.x += BSMOD_GRID_SIDE_MENU_SPACE;
        bsmod_instanceSideMenu(center, bs_v2(side_menu_width, dimensions.y));
    }

    if (bsmod.dragging_subtype >= 0) {
        bsmod_instanceDraggingIcon();
    }

    bsmod_instanceBillboards();
}
