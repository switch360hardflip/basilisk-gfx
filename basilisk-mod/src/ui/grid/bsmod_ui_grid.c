#include <direct.h>
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>

#include <bsgfx.h>
#include <bs_json.h>
#include <bs_window.h>
#include <bsmod_compile.h>
#include <bsmod_cache.h>
#include <bsmod.h>

#include <ui/bsgfx_ui.h>
#include <ui/bsmod_ui.h>
#include <ui/material/bsmod_ui_material.h>
#include <ui/side/bsmod_ui_side.h>
#include <ui/primitive/bsmod_ui_primitive.h>
#include <ui/tile/bsmod_ui_tile.h>
#include <ui/prefab/bsmod_ui_prefab.h>

#include <types/prefab/bsgfx_prefab.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/tile/bsgfx_tile.h>

#define BSMOD_DIRECTORY_BACKGROUND_PADDING 32
#define BSMOD_DIRECTORY_BACKGROUND_DIMENSIONS ((bs_vec2) { 256, 512 })

bool bsmod_instance_grid_menu = false;
bs_String* bsmod_search_input = NULL;

static void bsmod_onCloseGridMenu() {
    bsmod_instance_grid_menu = false;
}

typedef enum {
    BSMOD_DIRECTORY_NONE,

    BSMOD_DIRECTORY_TYPES,
        BSMOD_DIRECTORY_PRIMITIVES,
        BSMOD_DIRECTORY_PREFABS,
        BSMOD_DIRECTORY_TILES,
        BSMOD_DIRECTORY_SPAWNERS,
    BSMOD_DIRECTORY_MATERIALS,
        BSMOD_DIRECTORY_MATERIALS_CACHED,
        BSMOD_DIRECTORY_MATERIALS_STORED,
        BSMOD_DIRECTORY_MATERIALS_OTHER,

    BSMOD_DIRECTORY_COUNT
} bsmod_Directory;

static bsmod_Directory bsmod_selected_directory;
static void bsmod_onHoverDirectoryWidget(bsgfx_Widget* widget) {
    if (bsmod_leftClickOnce()) {
        bsmod_selected_directory = *(bsmod_Directory*)widget->params;
    }
}

static void bsmod_directoryWidget(bs_List* widgets, const char* name, int indent, bsmod_Directory* directory) {
    const int z_offset = 4;
    // these icons are all different sizes so manually shifting them a couple of pixels
    const int minimize_icon_y_offset = 0;
    const int folder_icon_y_offset = 0;
    const float align_height = 16.0;

    bool is_selected = bsmod_selected_directory == *directory;

    bs_pushBack(widgets, &(bsgfx_Widget) {
       // .name = "",
        .type = BSGFX_WIDGET_ICON,
        .align_height = align_height,
        .icon = {
            .atlas = bs_fetch(_bsmod_atlases, BSMOD_ATLAS_UI)->atlas,
            .atlas_subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
            .type = BSGFX_ICON_ATLAS,
            .name = "minimize",
            .hover = bsmod_onHoverDirectoryWidget,
            .material_id = BSMOD_UI_BASE_TEXT_MATERIAL()->id,
        },
        .params = directory,
        .offset = { indent, minimize_icon_y_offset, z_offset },
        .material_id = $bsmod_grey_138()->id,
        .advance_flags = BSGFX_WIDGET_ADVANCE_RIGHT,
    });

    bs_pushBack(widgets, &(bsgfx_Widget) {
       // .name = "",
        .type = BSGFX_WIDGET_ICON,
        .align_height = align_height,
        .icon = {
            .atlas = bs_fetch(_bsmod_atlases, BSMOD_ATLAS_UI)->atlas,
            .atlas_subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
            .type = BSGFX_ICON_ATLAS,
            .name = "folder",
            .hover = bsmod_onHoverDirectoryWidget,
        },
        .params = directory,
        .offset = { indent, folder_icon_y_offset, z_offset },
        .material_id = BSMOD_UI_BASE_TEXT_MATERIAL()->id,
        .advance_flags = BSGFX_WIDGET_ADVANCE_RIGHT,
    });


    if (is_selected) {
        bs_Font* font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head;
        bs_vec2 text_size = bs_textDimensions(font, name, strlen(name));
        bs_pushBack(widgets, &(bsgfx_Widget) {
            .type = BSGFX_WIDGET_BACKGROUND,
            .background = {
                .border_radius = BSMOD_DEFAULT_RADIUS,
                .size = text_size,
                .material_id = BSMOD_UI_BASE_TEXT_MATERIAL()->id,
                .subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_DITHER],
                .outline_material_id = $bsmod_grey_30()->id,
            },
            .offset = { indent, font->min_y_shift, z_offset },
            .material_id = $bsmod_grey_100()->id,
        });
    }
        bs_pushBack(widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_STRING,
        .align_height = align_height,
        .string = {
            .value = name,
            .on_hover = bsmod_onHoverDirectoryWidget,
        },
        .params = directory,
        .offset = { indent, 0, z_offset },
        .advance_flags = BSGFX_WIDGET_ADVANCE_DOWN,
    });


    bsgfx_Widget* last_widget = bs_fetchLast(widgets);
    last_widget->advance_flags |= BSGFX_WIDGET_ADVANCE_DOWN | BSGFX_WIDGET_ADVANCE_RESET_X;
}

static void bsmod_instanceDirectoryMenu(bs_vec3 center, bs_vec2 dimensions) {
    static bs_List widgets = { .unit_size = sizeof(bsgfx_Widget), .increment = 16 };
    widgets.count = 0;

    const int indent_0 = 8;
    const int indent_1 = 16;
    const int indent_2 = 40;

    static int select_position, select_size;

    const int search_height = 16;

    bs_pushBack(&widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_INPUT,
        .input = {
            .type = BSGFX_INPUT_STRING,
            .dimensions = { BSMOD_DIRECTORY_BACKGROUND_DIMENSIONS.x, search_height },
            .as_string = &bsmod_search_input,
            .placeholder_text = "Search...",
            .placeholder_text_material_id = $bsmod_grey_120()->id,
            .outline_material_id = $bsmod_grey_61()->id,
            .background_outline_material_id = $bsmod_grey_30()->id,
            .select_position = &select_position,
            .select_size = &select_size,
        },
        .material_id = $bsmod_grey_120()->id,
        .offset = { indent_0, -8.0, 0.0 },
        .advance_flags = BSGFX_WIDGET_ADVANCE_DOWN | BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Y,
    });

    bs_pushBack(&widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_BACKGROUND,
        .background = {
            .border_radius = BSMOD_DEFAULT_RADIUS,
            .size = BSMOD_DIRECTORY_BACKGROUND_DIMENSIONS,
            .material_id = $bsmod_grey_61()->id,
            .outline_material_id = $bsmod_grey_30()->id,
            .subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI_COLOR],
        },
        .offset = { indent_0, 0.0, 0 },
        .advance_flags = 0,
    });

    struct {
        const char* name;
        int indent;
        bsmod_Directory id;
    } directories[BSMOD_DIRECTORY_COUNT] = {
         [BSMOD_DIRECTORY_TYPES] = { "Types", indent_1 },
             [BSMOD_DIRECTORY_PRIMITIVES] = { "Primitives", indent_2 },
             [BSMOD_DIRECTORY_PREFABS] = { "Prefabs", indent_2 },
             [BSMOD_DIRECTORY_TILES] = { "Tiles", indent_2 },
             [BSMOD_DIRECTORY_SPAWNERS] = { "Spawners", indent_2 },
         [BSMOD_DIRECTORY_MATERIALS] = { "Materials", indent_1 },
             [BSMOD_DIRECTORY_MATERIALS_CACHED] = { "Cached", indent_2 },
             [BSMOD_DIRECTORY_MATERIALS_STORED] = { "Stored", indent_2 },
             [BSMOD_DIRECTORY_MATERIALS_OTHER] = { "Other", indent_2 },
    };

    for (int i = 0; i < BSMOD_DIRECTORY_COUNT; i++) {
        if (!directories[i].name)
            continue;
        directories[i].id = i;

        bsmod_directoryWidget(&widgets, directories[i].name, directories[i].indent, &directories[i].id);
    }

     /**
      Tabs
      */
    static bs_List tabs = { .unit_size = sizeof(bsgfx_MenuTab), .increment = 16 };
    static int active_tab;
    if (tabs.count == 0) {
        int material = $bsmod_grey_30()->id;

        bs_pushBack(&tabs, &(bsgfx_MenuTab) {.name = "Resources", });
        bs_pushBack(&tabs, &(bsgfx_MenuTab) {.name = "Console", });
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

     /**
      Instance
      */
    bool hovering = bsgfx_instanceWidgets((bsgfx_Menu) {
        .position = center,
        .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head,
        .text_subtype = _bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS],
        .spacing = 4.0,
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
    }, 
    NULL,
    //&(bsgfx_TitleBar) {
    //   // .name = "Window",
    //    .border_radius = BSMOD_DEFAULT_RADIUS,
    //    .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16),
    //    .material_id = $bsmod_grey_120()->id,
    //    .button_hovering_material_id = $bsmod_red()->id,
    //    .button_icon_material_id = $bsmod_grey_120()->id,
    //    .button_unavailable_material_id = $bsmod_grey_148()->id,
    //    .button_material_id = $bsmod_grey_61()->id,
    //    .button_shadow_material_id = $bsmod_grey_112()->id,
    //    .on_close = bsmod_onCloseGridMenu,
    //}, 
    &(bsgfx_MenuTabBar) {
        .tabs_count = tabs.count,
        .tabs = tabs.data,
        .active_tab = &active_tab,
        .material_id = $bsmod_grey_61()->id,
        .outline_material_id = $bsmod_grey_61()->id,
        .shadow_material_id = $bsmod_grey_30()->id,
        .height = BSMOD_TAB_BAR_HEIGHT,
    });

    if (!bsmod.ui_blocked)
        bsmod.ui_blocked = hovering;
}

static bool bsmod_instanceGridPreview(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering) {
    switch (bsmod_selected_directory) {
    case BSMOD_DIRECTORY_PRIMITIVES: bsmod_instancePrimitivePreview(widget, position, id, hovering); break;
    case BSMOD_DIRECTORY_PREFABS: bsmod_instancePrefabPreview(widget, position, id, hovering); break;
    case BSMOD_DIRECTORY_TILES: bsmod_instanceTilePreview(widget, position, id, hovering); break;
    default:
        return false;
    }

    if (hovering) {
        bsgfx_instanceQuad(
            bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
            bsgfx_matrix(
                BS_V3(position->x, position->y, 50.0),
                BS_V3(widget->grid.size.x, widget->grid.size.y, 0.0)
            ),
            $BSMOD_ATLAS_UI_grid_hover()->coords,
            0, 0, 0);
    }

    return true;
}

typedef struct {
    int id;
    int category;
} bsmod_IdCategory;

typedef struct {
    bsmod_SideMenuTabId tab_id;
    int subtype;
    bsmod_IdCategory* sorted_ids;
    bs_U32 source_id;
    bs_U32 object_id;
} bsmod_GridPreviewParams;

static void bsmod_checkHoverGrid(bsgfx_Widget* widget, bsgfx_GridParams* grid) {
    bsmod_GridPreviewParams* params = widget->params;
    bool hovering = bs_rectangleVsPoint(*grid->position, widget->grid.size, bs_windowCursorPosition());

    if (hovering) {
        if (bsmod_leftClickOnce()) {
            bsmod.dragging_id = grid->index;

            bsmod_setSideMenuTab(params->tab_id, (bsmod_GridClickParams) {
                .atlas_id = grid->index,
            });
        }

        if (bsmod_leftClickUpOnce()) {
            bsmod.dragging_id = -1;
        }

        bsgfx_instanceQuad(
            bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
            bsgfx_matrix(
                BS_V3(grid->position->x, grid->position->y, 50.0),
                BS_V3(widget->grid.size.x, widget->grid.size.y, 0.0)
            ),
            $BSMOD_ATLAS_UI_grid_hover()->coords,
            0, 0, 0);
    }
    else if (bsmod.dragging_id == grid->index) {
        bsmod.dragging_object_id = params->object_id;
        bsmod.dragging_subtype = params->subtype;
    }
}

static bool bsmod_instanceAtlasPreview(bsgfx_Widget* widget, bsgfx_GridParams grid) {

    bsmod_GridPreviewParams* params = widget->params;
    grid.index = params->sorted_ids[grid.index].id;

    bs_Atlas* atlas = bs_fetch(bsgfx_atlases(), params->object_id)->atlas;

    // special case
    if (params->object_id == BSMOD_ATLAS_MATERIAL_ICONS) {
        static int last_category = 0;

        bs_except(BSX_FAILED_TO_QUERY);
        bsgfx_Material* material = bsgfx_queryMaterial(atlas->unmapped[grid.index].name);
        if (bs_except(0))
            return false;

        if (last_category != material->category) {
            const char* category_name = bsgfx_materialCategoryName(material->category);
            if (category_name) {
                bs_Font* font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->font;
                grid.position->y -= font->height;
                grid.position->x = grid.start.x;
                bs_vec2 category_name_dimensions = bsgfx_textInstance(_bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS], font, &(bsgfx_Text) {
                    .position = { grid.position->x, grid.position->y, 70.0, 1 },
                    .scale = 16.0,
                    .material_id = $bsmod_grey_30()->id,
                }, category_name, strlen(category_name));

                grid.position->y -= widget->grid.size.y;
                //position->x = 0.0;
            }
        }

        last_category = material->category;
    }

    bsgfx_instanceQuad(
        params->subtype,
        bsgfx_matrix(
            BS_V3(grid.position->x, grid.position->y, 60.0),
            BS_V3(widget->grid.size.x, widget->grid.size.y, 0.0)
        ),
        bs_atlasCoordinates(atlas, grid.index, 0),
        0, 0, 0);

    bsmod_checkHoverGrid(widget, &grid);

    return true;
}

static bool bsmod_instanceImageArrayPreview(bsgfx_Widget* widget, bsgfx_GridParams grid) {
    bsmod_GridPreviewParams* params = widget->params;
    grid.index = params->sorted_ids[grid.index].id;

    bs_Image* image = bs_fetch(_bsmod_images, params->object_id)->image;
    bs_mat4x3 matrix = bsgfx_matrix(
        BS_V3(grid.position->x, grid.position->y, 60.0),
        BS_V3(widget->grid.size.x, widget->grid.size.y, 0.0)
    );
    matrix.f[8] = grid.index;
    bsgfx_instanceQuad(
        params->subtype,
        matrix,
        BS_V4(0.0, 0.0, 1.0, 1.0),
        0, 0, 0);

    bsmod_checkHoverGrid(widget, &grid);

    return true;
}

static int bsmod_compareCategories(const bsmod_IdCategory* a, const bsmod_IdCategory* b) {
    if (a->category == b->category) return 0;
    else if (a->category < b->category) return -1;
    else return 1;
}

void bsmod_instanceGridMenu(bs_vec3 position, bs_vec2 dimensions) {
    bsmod_instanceDirectoryMenu(position, dimensions);

    bs_vec2 size = { dimensions.x - (BSMOD_DIRECTORY_BACKGROUND_DIMENSIONS.x + BSMOD_DIRECTORY_BACKGROUND_PADDING), dimensions.y };

    static int scroll;

    bsmod_GridPreviewParams params = {
        .subtype = -1,
    };
    bsgfx_Widget widget = {
        .type = BSGFX_WIDGET_GRID,
        .grid = {
            .scrollbar = bsmod_scrollbar(&scroll),
            .always_active = true,
            .total_size = size,
            .count = BSGFX_PRIMITIVE_TYPE_COUNT,
            .action = bsmod_instanceGridPreview,
        },
        .params = &params,
    };

    widget.grid.count = 0;
    widget.grid.action = NULL;
    params.subtype = -1;

    bs_Atlas* atlas = NULL;
    bs_Image* image_array = NULL;
    float scale = 1.0;
    switch (bsmod_selected_directory) {
  //  case BSMOD_DIRECTORY_PREFABS: widget.grid.count = bsgfx_prefabModel()->meshes_count; break;
  //  case BSMOD_DIRECTORY_TILES: widget.grid.count = bsgfx_tileTypes()->count; break;
    case BSMOD_DIRECTORY_PREFABS:
        atlas = bs_fetch(_bsmod_atlases, params.object_id = BSMOD_ATLAS_PREFAB_ICONS)->head;
        widget.grid.action = bsmod_instanceAtlasPreview;
        widget.grid.count = atlas->count;
        params.subtype = _bsmod_subtypes[BSMOD_SUBTYPE_PREFAB_ICON];

        break;
    case BSMOD_DIRECTORY_PRIMITIVES:
        //bs_Atlas* atlas = bs_fetch(BSMOD_ATLAS_MATERIAL_ICONS)->head;
        atlas = bs_fetch(_bsmod_atlases, params.object_id = BSMOD_ATLAS_PRIMITIVE_ICONS)->head;
        widget.grid.count = atlas->count;
        widget.grid.action = bsmod_instanceAtlasPreview;
        params.subtype = _bsmod_subtypes[BSMOD_SUBTYPE_PRIMITIVE_ICON];

        break;
    case BSMOD_DIRECTORY_TILES: 
        image_array = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_TILE)->image;
        widget.grid.count = image_array->num_indices;
        widget.grid.action = bsmod_instanceImageArrayPreview;
        widget.grid.padding = 8.0;
        params.subtype = _bsmod_subtypes[BSGFX_SUBTYPE_TILE];
        params.object_id = BSGFX_IMAGE_TILE;
        scale = 2.0;
        break;
    case BSMOD_DIRECTORY_MATERIALS_CACHED: 
        bs_except(BSX_NOT_FOUND);
        atlas = bs_fetch(_bsmod_atlases, BSMOD_ATLAS_MATERIAL_ICONS)->atlas;
        if (!bs_except(0)) {
            widget.grid.count = atlas->count;
            widget.grid.action = bsmod_instanceAtlasPreview;
            params.subtype = _bsmod_subtypes[BSMOD_SUBTYPE_MATERIAL_ICON];
            params.tab_id = BSMOD_TAB_MATERIAL;
            params.object_id = BSMOD_ATLAS_MATERIAL_ICONS;
        }
        break;
    default:
        return;
    }

    if (widget.grid.count > 0) {
        if (atlas) {
            widget.grid.size = BS_V2(atlas->mapped[0].w, atlas->mapped[0].h);
            assert(atlas->count == widget.grid.count);
        }
        else if (image_array) {
            widget.grid.size = bs_v2Iv2(image_array->dim);
            assert(image_array->num_indices == widget.grid.count);
        }

        widget.grid.size.x *= scale;
        widget.grid.size.y *= scale;

        params.sorted_ids = _alloca(widget.grid.count * sizeof(bsmod_IdCategory));
        for (int i = 0; i < widget.grid.count; i++) {
            params.sorted_ids[i] = (bsmod_IdCategory){
                .id = i,
                .category = atlas ? atlas->unmapped[i].category : 0,
            };
        }

        qsort(params.sorted_ids, widget.grid.count, sizeof(bsmod_IdCategory), bsmod_compareCategories);
    }

    position.x += BSMOD_DIRECTORY_BACKGROUND_DIMENSIONS.x + BSMOD_DIRECTORY_BACKGROUND_PADDING;
    position.z += 10;

    bool hovering = bsgfx_instanceWidgets((bsgfx_Menu) {
        .position = position,
        .text_subtype = _bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS],
        .font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head,
        .spacing = 16.0,
        .widgets = &widget,
        .widgets_count = 1,
        .background_material_id_0 = $blank_material()->id,
        .background_material_id_1 = $blank_material()->id,
        .untextured = {
            .dimensions = size,
            .auto_scale_width = true,
            .border_id = -1,
            // .has_shadow = true,
        },
        .blocked = false,
        .border_radius = BSMOD_DEFAULT_RADIUS,
    }, NULL, NULL);

    if (!bsmod.ui_blocked)
        bsmod.ui_blocked = hovering;
}
