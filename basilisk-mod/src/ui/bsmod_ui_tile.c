#include <bs_types.h>
#include <types/tile/bsgfx_tile.h>
#include <ui/bsgfx_ui.h>
#include <bsmod_type.h>
#include <ui/bsmod_ui.h>
#include <ui/grid/bsmod_ui_grid.h>
#include <types/primitive/bsgfx_primitive.h>
#include <_bsmod_.h>



  /*==============================================================================
   * Instance Menu
   *============================================================================*/

static void bsmod_pushDividerWidget(bs_List* widgets, bs_vec3 offset) {
    const int divider_indent = 8;
    bs_pushBack(widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_BACKGROUND,
        .background = {
            .size = { BSGFX_SIDE_MENU_WIDTH - divider_indent * 2.0, 1.0 },
            .material_id = $bsmod_grey_30()->id,
            .subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI_COLOR],
        },
        .offset = { divider_indent, offset.y, offset.z + 1 },
        // .align_height = height,
    });
}

void bsmod_pushDropdownWidget(bs_List* widgets, bs_vec3 offset) {
    const int height = 24;

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_ICON,
        .icon = {
            .atlas = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_UI)->atlas,
            .atlas_subtype = bsgfx_subtypes()[BSGFX_SUBTYPE_UI],
            .type = BSGFX_ICON_ATLAS,
            .name = "minimize",
            .material_id = BSMOD_UI_BASE_TEXT_MATERIAL()->id,
        },
        .advance_flags = BSGFX_WIDGET_ADVANCE_RIGHT | BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_X,
        .align_height = height,
        .offset = offset,
    });

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .type = BSGFX_WIDGET_STRING,
        .string = {
            .value = "Test",
        },
        .align_height = height,
    });

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .advance_flags = BSGFX_WIDGET_ADVANCE_RESET_X | BSGFX_WIDGET_ADVANCE_APPLY_OFFSET,
        .offset = { 0.0, -height, 0.0 },
    });

    bsmod_pushDividerWidget(widgets, BS_V3(0.0, offset.y, offset.z));
}

void bsmod_pushTileMenuWidgets(bs_List* widgets, bs_vec2 background_size) {
    const int indent = 24;

    if (_bsmod_.selected_ids.count <= 0) {
        bs_pushBack(widgets, &(bsgfx_Widget) {
            .type = BSGFX_WIDGET_STRING,
            .string = {
                .value = "No tiles selected",
            },
            .offset.x = indent,
        });
        return;
    }

    bs_pushBack(widgets, &(bsgfx_Widget) {
        .advance_flags = BSGFX_WIDGET_ADVANCE_APPLY_OFFSET,
        .offset = { 0, -8.0, 0.0 },
    });

    bsmod_pushDividerWidget(widgets, BS_V3(0.0, 0.0, 0.0));

    for (int i = 0; i < _bsmod_.selected_ids.count; i++) {
        bsgfx_Tile* tile = bsgfx_get(BSGFX_TYPE_TILE, i);

        bsmod_pushDropdownWidget(widgets, BS_V3(indent, 0, 0));
    }
}



  /*==============================================================================
   * Grid Icon
   *============================================================================*/

bool bsmod_instanceTilePreview(bsgfx_Widget* widget, bs_vec2* position, int id, bool hovering) {
    bs_Image* image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_TILE)->image;
    bs_ImageIndex* index = image->indices + id;

    if (bsmod_search_input->len > 0 && strstr(index->name, bsmod_search_input->value) == NULL) {
        return false;
    }

    bs_mat4x3 transform = bsgfx_matrix(BS_V3(position->x, position->y, 0.0), BS_V3(widget->grid.size.x, widget->grid.size.y, 0));
    transform.f[8] = id;
    bsgfx_instanceQuad(bsgfx_subtypes()[BSGFX_SUBTYPE_TILE_ICON], transform, BS_V4(0, 0, 1, 1), 0, 0, $white_material()->id);

    return true;
}



  /*==============================================================================
   * Dragging Icon
   *============================================================================*/

void bsmod_onDragTile(bsmod_DraggingParams params) {
    if (_bsmod_.hovering.tile < 0 || _bsmod_.hovering.primitive < 0)
        return;

    bsgfx_Primitive* hovering_primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, _bsmod_.hovering.primitive);

    bs_ivec2 coords = bsgfx_tileCoordinate(hovering_primitive, _bsmod_.hovering.tile_axis, _bsmod_.hovering.tile);

    bs_vec3 tile_position = bsgfx_tilePosition(hovering_primitive, _bsmod_.hovering.tile_axis, coords.x, coords.y);
    bs_vec4 rotation = bsgfx_tileRotation(_bsmod_.hovering.tile_axis);
    bs_vec3 euler_rotation = bsgfx_tileEulerRotation(_bsmod_.hovering.tile_axis);
    rotation = bs_qMulq(hovering_primitive->rotation, rotation);
    bs_mat4 matrix = bs_transform(tile_position, rotation, bs_v3V1(1.0));

    bs_Image* tile_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_TILE)->image;
    assert(_bsmod_.dragging_id >= 0 && _bsmod_.dragging_id < tile_image->num_indices);

    if (bs_leftClickUpOnce()) {
        if (bsmod_isSelected(BSMOD_TILE_IDS, BSGFX_TYPE_TILE, _bsmod_.hovering.tile)) {
            for (int i = 0; i < _bsmod_.selected_tiles.count; i++) {
                int id = *(int*)bs_fetchUnit(&_bsmod_.selected_tiles, i);

                for (int j = 0; j < _bsmod_.selected_ids.count; j++) {
                    int existing_id = *(int*)bs_fetchUnit(&_bsmod_.selected_ids, j);
                    bsgfx_Tile* tile = bsgfx_get(BSGFX_TYPE_TILE, existing_id);
                    if (id == tile->index) {
                        bsgfx_RawTile* raw_tile = bsgfx_getRaw(BSGFX_TYPE_TILE, existing_id);
                        raw_tile->texture_hash = tile_image->indices[_bsmod_.dragging_id].name_hash;
                        goto next;
                    }
                }

                coords = bsgfx_tileCoordinate(hovering_primitive, _bsmod_.hovering.tile_axis, id);

                bsmod_add(BSGFX_TYPE_TILE, &(bsgfx_RawTile) {
                    .coords = coords,
                    .texture_hash = tile_image->indices[_bsmod_.dragging_id].name_hash,
                    .primitive = hovering_primitive->guid,
                    .axis = _bsmod_.hovering.tile_axis,
                });

            next:
                void;
            }
        }
        else {
            bs_warnF("Tile is not selected\n");
        }

        bsmod_saveType(BSGFX_TYPE_TILE, "Created %d tiles", _bsmod_.selected_tiles.count);
        bsmod_deselectAll();
    }
}
