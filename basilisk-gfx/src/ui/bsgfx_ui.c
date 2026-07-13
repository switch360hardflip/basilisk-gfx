#include <bs_log.h>
#include <bs_collision.h>
#include <bsgfx_material.h>
#include <types/tile/bsgfx_tile.h>

#include <bsgfx.h>

#include <UI/bsgfx_ui.h>
#include <bsgfx_pipeline.h>

#define BSGFX_DEBUG_MENU_INPUT_COLOR (bs_RGBA) { 32, 32, 40, 255 }
#define BSGFX_DEBUG_MENU_INPUT_SELECTED_COLOR (bs_RGBA) { 24, 24, 32, 255 }

#define BSGFX_DEBUG_URL_COLOR (bs_RGBA) { 70, 80, 200, 255 }
#define BSGFX_DEBUG_URL_SELECTED_COLOR (bs_RGBA) { 100, 110, 255, 255 }



  /*==============================================================================
   * UI
   *============================================================================*/

static inline bs_mat4x3 bsgfx_roundedMatrix(bs_vec3 position, bs_vec3 scale, bs_vec4 radii) {
	return (bs_mat4x3) {
		scale.x, radii.x, radii.y,
		radii.z, scale.y, 0.0,
		radii.w, 0.0, scale.z,
		position.x, position.y, position.z,
	};
}

static void bsgfx_instanceBackground(
	int subtype,
	bs_vec3 position,
	bs_vec2 dimensions,
	bs_vec4 border_radii,
	int material_id,
	int outline_material_id,
	int shadow_material_id)
{
	bs_vec4 background_radii = border_radii;
	background_radii.z = background_radii.w = 0.0;

	bs_mat4x3 transform = bsgfx_roundedMatrix(position, bs_v3(dimensions.x, dimensions.y, 0.0), bs_v4V1(0));

	if (material_id) {
		bsgfx_instanceQuad(subtype, transform, bs_v4V1(0), 0, 0, material_id);
	}

	/**
	 Shadow
	 */
	if (shadow_material_id) {
		const float shadow_width = outline_material_id ? 2.0 : 1.0;
		bs_vec3 shadow_offset_1 = bs_v3(-shadow_width, -shadow_width, -2);
		bs_vec3 shadow_position;
		bs_v3Add(&position, &shadow_offset_1, &shadow_position);
		bs_vec3 shadow_size = bs_v3(dimensions.x + shadow_width * 2.0, dimensions.y + shadow_width * 2.0, 0.0);

		transform = bsgfx_roundedMatrix(shadow_position, shadow_size, border_radii);
		bsgfx_instanceQuad(subtype, transform, bs_v4V1(0), 0, 0, shadow_material_id);
	}

	/**
	 Outline
	 */
	if (outline_material_id) {
		const float shadow_width = 1.0;
		bs_vec3 shadow_offset_2 = bs_v3(-shadow_width, -shadow_width, -1);
		bs_vec3 shadow_position;
		bs_v3Add(&position, &shadow_offset_2, &shadow_position);
		bs_vec3 shadow_size = bs_v3(dimensions.x + shadow_width * 2.0, dimensions.y + shadow_width * 2.0, 0.0);

		transform = bsgfx_roundedMatrix(shadow_position, shadow_size, border_radii);
		bsgfx_instanceQuad(subtype, transform, bs_v4V1(0), 0, 0, outline_material_id);
	}
}

#define BSGFX_PIXEL_SCALE 1.0
bool bsgfx_rectangleVsPointExpand(bs_vec2 position, bs_vec2 dimensions, bs_vec2 point, float expand) {
	expand *= BSGFX_PIXEL_SCALE;
	position = bs_v2AddV1(position, -expand);
	dimensions = bs_v2AddV1(dimensions, expand * 2.0);
	return bs_rectangleVsPoint(position, dimensions, point);
}

static inline float bsgfx_textHeight() {
	return 0.0;
	bs_Font* font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head;
	return font->height;
}

static inline float bsgfx_instanceTextField(int subtype, bs_vec3* position, char* text, int material_id) {
	return 0.0;
	bs_Font* font = bs_fetch(BSGFX_FONTS, BSGFX_FONT_ARIAL_16)->head;
	//position->y -= bsgfx_textHeight();

	return bsgfx_textInstance(subtype, font, &(bsgfx_Text) {
		.position = { position->x, position->y, position->z + 1, 1 },
		.scale = font->size,
		.material_id = material_id,
	}, text, strlen(text)).x;
}

static inline float bsgfx_instanceTextFieldV(int subtype, bs_vec3* position, int material_id, const char* format, va_list args) {
	char formatted[256];
	vsnprintf(formatted, sizeof(formatted), format, args);
	return bsgfx_instanceTextField(subtype, position, formatted, material_id);
}

float bsgfx_instanceTextFieldF(int subtype, bs_vec2* position, int material_id, const char* format, ...) {
	va_list args;
	va_start(args, format);
	float width = bsgfx_instanceTextFieldV(subtype, position, material_id, format, args);
	va_end(args);
	return width;
}

static void bsgfx_instanceScrollbar(bsgfx_Scrollbar* scrollbar, bs_vec3 position, int max_scroll, int scroll, float total_height) {
	position.x -= scrollbar->width;

	 /**
	  Buttons
	  */
	bool draw_buttons = scrollbar->button_icon_material > 0;
	if (draw_buttons) {
		total_height -= scrollbar->width * 2.0;

		 /**
		  Icons
		  */
	//	bsgfx_AtlasCache* button = $BSMOD_ATLAS_UI_scrollbar_button();
	//	position.y -= scrollbar->width;
	//	bs_vec3 icon_position = {
	//		position.x + (scrollbar->width - button->size.x) / 2.0,
	//		position.y + (scrollbar->width - button->size.y) / 2.0,
	//		position.z + BSGFX_BACKGROUND_Z_COUNT
	//	};

		 /**
		  Backgrounds
		  */
		bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
			bs_v3(position.x, position.y, position.z + 2),
			bs_v2(scrollbar->width, scrollbar->width),
			bs_v4V1(scrollbar->border_radius),
			scrollbar->button_background_material,
			scrollbar->button_outline_material,
			0);

		bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
			bs_v3(position.x, position.y - total_height - scrollbar->width, position.z + 2),
			bs_v2(scrollbar->width, scrollbar->width),
			bs_v4V1(scrollbar->border_radius),
			scrollbar->button_background_material,
			scrollbar->button_outline_material,
			0);

	//	bs_mat4x3 icon_matrix = bsgfx_roundedMatrix(icon_position, bs_v3V2(button->size, 0.0), bs_v4V1(0.0));
	//	bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI], icon_matrix, button->coords, 0, 0, scrollbar->button_icon_material);
	//	icon_matrix.v[3].y -= total_height + scrollbar->width;
	//	bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI], icon_matrix, bs_flipUV(button->coords), 0, 0, scrollbar->button_icon_material);
	}

	 /**
	  Scrollbar background
	  */
	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3(position.x, position.y - total_height, position.z + 1),
		bs_v2(scrollbar->width, total_height),
		bs_v4V1(scrollbar->border_radius),
		scrollbar->background_material,
		0,
		0);

	 /**
	  Scrollbar
	  */
	if (draw_buttons) {
		const int padding = 2;
		position.y -= padding;
		total_height -= padding * 2;
	}

	float height = total_height / (float)(max_scroll + 1);
	position.y -= height * (scroll + 1);

	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3(position.x, position.y, position.z + BSGFX_BACKGROUND_Z_COUNT),
		bs_v2(scrollbar->width, height),
		bs_v4V1(scrollbar->border_radius),
		scrollbar->material,
		scrollbar->outline_material,
		0);
}

/**
 URL
 */
static bool bsgfx_instanceUrl(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, int id, bs_vec3 position, bs_vec2* out_width) {
	bs_vec2 cursor = bs_windowCursorPosition();
	bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
	bs_Font* font = widget->font ? widget->font : menu->font;
	int ui_copy = bs_queryAtlas(atlas, "ui_copy");
	bs_vec2 ui_copy_size = bs_atlasSize(atlas, ui_copy);

	//if ((position->y - ui_copy_size.y) < menu->coords.y)
	//	return;

	bs_vec2 name_dimensions = bs_textDimensions(font, widget->url.path, strlen(widget->url.path));
	bool url_hovering = !already_hovering && bs_rectangleVsPoint(bs_v2AddY(position.xy, -name_dimensions.y), name_dimensions, cursor);
	bsgfx_instanceTextField(menu->text_subtype, &position, widget->url.path, /*url_hovering ? BSGFX_DEBUG_URL_SELECTED_COLOR : BSGFX_DEBUG_URL_COLOR */ 0);

	bs_vec3 copy_position = bs_v3(position.x + name_dimensions.x, position.y, position.z + 1);
	bs_vec2 copy_dimensions;
	bs_v2MulV1(&ui_copy_size, BSGFX_PIXEL_SCALE, &copy_dimensions);
	bool copy_hovering = !already_hovering && bs_rectangleVsPoint(copy_position.xy, copy_dimensions, cursor);

	bs_mat4 transform = bs_transform(copy_position, BS_QUAT_IDENTITY, bs_v3(copy_dimensions.x, copy_dimensions.y, 0));
	bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS_HI], bs_m4x3(transform), bs_atlasCoordinates(atlas, ui_copy, 0), 0, 0, 0);

	//bsgfx_atlasLegacyHiResInstance(atlas, copy_position, ui_copy, bs_v2V1(BSGFX_PIXEL_SCALE), BS_WHITE, false);

	if (bs_leftClickOnce()) {
		if (url_hovering) {
			bs_throwBasilisk(BSX_NOT_IMPLEMENTED);
			bs_systemF(false, "explorer /select,\"%s\"", widget->url.path);
		}
		else if (copy_hovering) {
			bs_String* string = bs_fullPath(NULL, widget->url.path, strlen(widget->url.path));
			bs_replaceCharOccurrences(string->value, string->len, '\\', '/');
			bs_copyToClipboard(string->value, string->len);
			bs_free(string);
		}
	}

	*out_width = name_dimensions;

	return url_hovering || copy_hovering;
}

/**
 Range
 */
static bool bsgfx_instanceRange(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, int id, bs_vec3 position, bs_vec2* out_width) {
	bs_vec2 cursor = bs_windowCursorPosition();
	bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
	bs_Font* font = widget->font ? widget->font : menu->font;
	int ui_increment = bs_queryAtlas(atlas, "ui_increment");

	//bs_vec2 name_dimensions = bs_textDimensions(font, widget->name, strlen(widget->name));
	bs_vec2 name_dimensions = bs_v2(16, 16);

	bs_vec3 copy_position = bs_v3(position.x + name_dimensions.x, position.y, position.z + 1);
	bs_vec2 ui_increment_size = bs_atlasSize(atlas, ui_increment);
	bs_vec2 copy_dimensions;
	bs_v2MulV1(&ui_increment_size, BSGFX_PIXEL_SCALE, &copy_dimensions);

	bs_mat4x3 m = bsgfx_matrix(copy_position, bs_v3(copy_dimensions.x, copy_dimensions.y, 0.0));
	bsgfx_atlasInstanceFlipped(_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS_HI], m, ui_increment, 0, 0, $white_material()->id);

	bool hovering = !already_hovering && bs_rectangleVsPoint(copy_position.xy, copy_dimensions, cursor) && bs_leftClickOnce();
	if (hovering) {
		(*widget->range.value)--;
		if (widget->range.on_change)
			widget->range.on_change(widget);
	}

	copy_position.x += copy_dimensions.x + BSGFX_PIXEL_SCALE;
	bs_vec2 text_position = copy_position.xy;

	const bsgfx_Text text = {
		.position = { copy_position.x, copy_position.y, position.z + 1, 1 },
		.scale = BSGFX_PIXEL_SCALE,
	};

	float x = bsgfx_textInstanceF(font, &text, "%d/%d", *widget->range.value, widget->range.max).x;
	copy_position.x += x;

	if (bs_rectangleVsPoint(copy_position.xy, copy_dimensions, cursor) && bs_leftClickOnce()) {
		(*widget->range.value)++;
		if (widget->range.on_change)
			widget->range.on_change(widget);
	}

	m = bsgfx_matrix(copy_position, bs_v3(copy_dimensions.x, copy_dimensions.y, 0.0));
	bsgfx_atlasInstance(_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS_HI], m, ui_increment, 0, 0, $white_material()->id);

	if ((*widget->range.value) <= widget->range.min)
		(*widget->range.value) = widget->range.min;
	if ((*widget->range.value) > widget->range.max)
		(*widget->range.value) = widget->range.max;

	*out_width = copy_dimensions;

	return hovering;
}

/**
 Slider
 */
static bool bsgfx_instanceSlider(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, int id, bs_vec3 position, bs_vec2* out_size) {
	bs_vec2 cursor = bs_windowCursorPosition();
	bs_Font* font = widget->font ? widget->font : menu->font;

//	bs_vec2 name_dimensions = bs_textDimensions(font, widget->name, strlen(widget->name));
	bs_vec2 name_dimensions = bs_v2(16, 16);
	bs_vec2 bar_dimensions = bs_v2(3 * BSGFX_PIXEL_SCALE, 4.0 * BSGFX_PIXEL_SCALE);
	position.y -= bar_dimensions.y;

	bs_vec3 copy_position = bs_v3(position.x + name_dimensions.x, position.y, position.z + 1);
	bs_vec2 text_position = copy_position.xy;

	const float default_width = 16.0;
	float width = (widget->slider.width == 0.0 ? default_width : widget->slider.width) * BSGFX_PIXEL_SCALE;

	float value_norm = (*widget->slider.value - widget->slider.min) / (widget->slider.max - widget->slider.min);
	value_norm = bs_clamp(value_norm, 0.0f, 1.0f);

	bs_vec3 bar_offset = bs_v3(value_norm * width, BSGFX_PIXEL_SCALE, 0.0);
	bs_vec3 bar_position;
	bs_v3Add(&copy_position, &bar_offset, &bar_position);
	bs_mat4x3 m = bsgfx_matrix(bs_v3AddY(copy_position, bar_dimensions.y / 2.0), bs_v3(width, BSGFX_PIXEL_SCALE, 0.0));
	bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR], m, bs_v4(0, 0, 1, 1), 0, 0, $white_material()->id);
	m = bsgfx_matrix(bar_position, bs_v3(bar_dimensions.x, bar_dimensions.y, 0.0));
	bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR], m, bs_v4(0, 0, 1, 1), 0, 0, $white_material()->id);

	static bool dragging;

	if (!bs_leftClick())
		dragging = false;

	if (bsgfx_rectangleVsPointExpand(bar_position.xy, bar_dimensions, cursor, 0.0) && bs_leftClickOnce())
		dragging = true;

	if (dragging) {
		float diff = cursor.x - copy_position.x;
		float scale = bs_clamp(diff / width, 0.0f, 1.0f);

		*widget->slider.value = widget->slider.min + scale * (widget->slider.max - widget->slider.min);
		if (widget->slider.integer)
			*widget->slider.value = roundf(*widget->slider.value);
	}

	copy_position.x += width + BSGFX_PIXEL_SCALE;

	*out_size = bs_v2(width, bar_dimensions.y);

	return false;
}

/**
 Icon
 */
static bool bsgfx_instanceIcon(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, int id, bs_vec3 position, bs_vec2* out_size) {
	int border_size = (bool)widget->icon.outline_material_id;
	//position->y -= border_size;

	bs_vec2 cursor = bs_windowCursorPosition();

	bs_Font* font = widget->font ? widget->font : menu->font;

	int ui_icon = 0;
	if (widget->icon.type == BSGFX_ICON_ATLAS)
		ui_icon = bs_queryAtlas(widget->icon.atlas, widget->icon.name);

	bs_vec2 size = widget->icon.scale;
	if (widget->icon.scale.x != 0.0)
		size = widget->icon.scale;
	else if (widget->icon.type == BSGFX_ICON_TILE)
		size = BSGFX_TILE_SIZE;
	else if (widget->icon.type == BSGFX_ICON_ATLAS) 
		size = bs_atlasSize(widget->icon.atlas, ui_icon);
	else if (widget->icon.type == BSGFX_ICON_MATERIAL) {
		//bsgfx_Material* material = bs_fetchUnit(bsgfx_materials(), widget->icon.material_id);
		//bsgfx_Texture* texture = bsgfx_fetchTexture(material->contract->scale, material->contract->image);
		//size = bs_v2Iv2(texture->size);
		size = bs_v2(256, 256); // todo
	}

	//bs_vec2 name_dimensions = { 0 };
	//if (widget->name)
	//	name_dimensions = bs_textDimensions(font, widget->name, strlen(widget->name));

	//if (widget->icon.placement == BSGFX_ICON_PLACE_BELOW)
	//	position->y -= size.y;


	if (widget->align_height == 0.0)
		widget->align_height = size.y;
	position.y -= (widget->align_height + size.y) / 2.0;
	bs_vec3 icon_position = bs_v3(position.x /* + (widget->icon.placement == BSGFX_ICON_PLACE_BELOW || widget->icon.placement == BSGFX_ICON_PLACE_LEFT ? 0.0 : name_dimensions.x)*/, position.y, position.z + 1);
	bool hovering = !already_hovering && bs_rectangleVsPoint(icon_position.xy, size, cursor);

	bs_mat4 transform;
	switch (widget->icon.type) {
	case BSGFX_ICON_TILE:
		transform = bs_transform(icon_position, BS_QUAT_IDENTITY, bs_v3(size.x, size.y, 0));
		bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_TILE_HI], bs_m4x3(transform), bs_v4(0, 0, 1, 1), 0, 0, widget->icon.material_id);
		//bsgfx_tileHiResInstance(icon_position, bsgfx_queryTileType(widget->icon.name), BS_WHITE);
		//dux_tileHiResInstance(icon_position, bsgfx_queryTileType(widget->icon.name), size, hovering ? bs_v4MulV1(color, 2.0) : color);
		break;
	case BSGFX_ICON_ATLAS:
		bs_Atlas* atlas = widget->icon.atlas;
		transform = bs_transform(icon_position, BS_QUAT_IDENTITY, bs_v3(size.x, size.y, 0));
		if (widget->icon.background_name) {
			bs_vec4 background_coords = bs_atlasCoordinates(atlas, bs_queryAtlas(atlas, widget->icon.background_name), widget->icon.frame);
			bsgfx_instanceQuad(widget->icon.atlas_subtype, bs_m4x3(transform), background_coords, 0, 0, 0);
		}

		transform.v[3].z++;
		bs_vec4 coords = bs_atlasCoordinates(atlas, bs_queryAtlas(atlas, widget->icon.name), widget->icon.frame);
		if (widget->icon.flipped) coords = bs_flipUV(coords);
		if (widget->icon.mirrored) coords = bs_mirrorUV(coords);
		bsgfx_instanceQuad(widget->icon.atlas_subtype, bs_m4x3(transform), coords, 0, 0, widget->icon.material_id);
		break;
	case BSGFX_ICON_MATERIAL:
		bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
			icon_position,
			size,
			bs_v4V1(widget->icon.border_radius),
			0,
			widget->icon.outline_material_id,
			0);

		icon_position.z += BSGFX_BACKGROUND_Z_COUNT;
		transform = bs_transform(icon_position, BS_QUAT_IDENTITY, bs_v3(size.x, size.y, 0));
		bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_QUAD_MATERIAL_TEXTURE], bs_m4x3(transform), bs_v4(0, 0, 1, 1), 0, 0, widget->icon.material_id);
		break;
	}

	if (widget->icon.hover && hovering)
		widget->icon.hover(widget);

	/*
	if (widget->icon.placement == BSGFX_ICON_PLACE_LEFT)
		position->x += size.x;



	if (widget->prefer_x) {
		//position->y += size.y;
		if (widget->icon.placement != BSGFX_ICON_PLACE_LEFT)
			position->x += BS_MAX(size.x, name_dimensions.x);
	}
	*/

	*out_size = size;

	return hovering;
}

/**
 List
 */
static bool bsgfx_instanceList(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, int id, bs_vec3 position, bs_vec2* out_size) {
	bs_vec2 cursor = bs_windowCursorPosition();
	bs_Font* font = widget->font ? widget->font : menu->font;

	const int max_name_length = 128;

	bs_vec3 p = bs_v3AddZ(position, 5);
	p.y -= BSGFX_LIST_ROW_DIMENSIONS.y;

	int offset = widget->list.scroll ? *widget->list.scroll : 0;
	offset += widget->list.vk_offset;

	/*
	if (widget->list.scroll)
		bsgfx_instanceScrollbar(
			menu,
			bs_v3(position->x + BSGFX_LIST_ROW_DIMENSIONS.x, position->y, position->z),
			0, // todo
			widget->list.scroll,
			BSGFX_LIST_ROW_DIMENSIONS.y,
			0, // todo
			widget->list.count,
			widget->list.max);
			*/
	for (int i = 0; i < BS_MIN(widget->list.max, widget->list.count - offset); i++) {
		p.x = position.x;
		char* row_string = widget->list.foreach_visible_row((bsgfx_ForeachVisibleRowParams) {
			.widget = widget,
			.position = &p,
			.id = i + offset,
		});
		if (!row_string)
			continue;

		p.y += bsgfx_textHeight();

		if (strlen(row_string) > max_name_length) {
			char old = row_string[max_name_length - 1];
			row_string[max_name_length - 1] = '\0';
			bsgfx_instanceTextFieldF(&p, 0, "%s...", row_string);
			row_string[max_name_length - 1] = old;
		}
		else
			bsgfx_instanceTextField(menu->text_subtype, &p, row_string, 0);
		p.y -= BSGFX_LIST_ROW_DIMENSIONS.y;
	}

	bs_vec2 dimensions = bs_v2(BSGFX_LIST_ROW_DIMENSIONS.x, BSGFX_LIST_ROW_DIMENSIONS.y * (BS_MIN(widget->list.max, widget->list.count) + 1));
	position.y -= dimensions.y - BSGFX_LIST_ROW_DIMENSIONS.y;
	//bsgfx_instanceSquare(bs_v3AddZ(*position, 1), dimensions, bs_rgba(80, 90, 90, 100));

	*out_size = dimensions;
	return !already_hovering && bs_rectangleVsPoint(position.xy, dimensions, cursor);
}

 /**
  String
  */
/*
static bool bsgfx_instanceWidgetName(bsgfx_Menu* menu, bsgfx_Widget* widget, bs_vec2 cursor) {
	if (!widget->name)
		return false;

	int widget_name_len = strlen(widget->name);
	if (widget_name_len <= 0)
		return false;

	bs_textDimensions(widget->font ? widget->font : menu->font, widget->name, widget_name_len);
	if (widget->type != BSGFX_WIDGET_BUTTON &&
		widget->type != BSGFX_WIDGET_URL)
	{
		bs_vec3 position = menu->position;
		float width = bsgfx_instanceTextField(menu->text_subtype, &position, widget->name, widget->material_id);
		bs_Font* font = widget->font ? widget->font : menu->font;
		bs_vec2 size = bs_v2(width, font->height);

		menu->position.x += width;
		//if (widget->prefer_x)
		//	menu->position.x += size.x;
		return bs_rectangleVsPoint(position.xy, size, cursor);
	}

	return false;
}
*/

static bool bsgfx_instanceString(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, bs_vec3 position, bs_vec2* out_width) {
	bs_vec2 cursor = bs_windowCursorPosition();

	//if (widget->type != BSGFX_WIDGET_ICON || widget->icon.placement == BSGFX_ICON_PLACE_RIGHT || widget->icon.placement == BSGFX_ICON_PLACE_BELOW)
	//hovering_name = bsgfx_instanceWidgetName(&menu, widget, cursor);

	bs_Font* font = widget->font ? widget->font : menu->font;
	int len = strlen(widget->string.value);
	bs_vec2 text_dimensions = bs_textDimensions(font, widget->string.value, len);

	if (widget->align_height == 0.0)
		widget->align_height = text_dimensions.y;

	position.y -= (widget->align_height + text_dimensions.y) / 2.0 + font->min_y_shift;
	const bsgfx_Text text = {
		.position = {
			position.x,
			position.y,
			position.z + 2, 1
		},
		.scale = font->size,
	};

	bsgfx_textInstance(menu->text_subtype, font, &text, widget->string.value, len);
	bool hovering = bs_rectangleVsPoint(text.position.xy, text_dimensions, bs_windowCursorPosition());
	if (widget->string.on_hover && hovering)
		widget->string.on_hover(widget);

	*out_width = text_dimensions;

	return hovering;
}

 /**
  Button
  */
static bool bsgfx_instanceButton(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, bs_vec3 position, bs_vec2* out_width) {
	bs_vec2 cursor = bs_windowCursorPosition();
	bs_vec2 size = widget->button.size;

	bs_Font* font = widget->font ? widget->font : menu->font;

	bs_vec2 text_dimensions;
	if (widget->button.name) {
		text_dimensions = bs_textDimensions(font, widget->button.name, strlen(widget->button.name));

		if (size.x == 0)
			size.x = text_dimensions.x;
		if (size.y == 0)
			size.y = text_dimensions.y;
	}

	if (menu->untextured.auto_scale_width)
		menu->untextured.dimensions.x = BS_MAX(menu->untextured.dimensions.x, size.x);

	position.y -= size.y;

	bool hovering =
		(widget->button.active_override.active_index && *widget->button.active_override.active_index == widget->button.active_override.this_index) ||
		(!already_hovering && bs_rectangleVsPoint(position.xy, size, cursor));

	if (widget->button.name) {
		const bsgfx_Text text = {
			.position = {
				position.x + (size.x - text_dimensions.x) / 2.0,
				position.y + (size.y - text_dimensions.y) / 2.0,
				position.z + 2, 1
			},
			.scale = font->size,
		};

		bsgfx_textInstance(menu->text_subtype, font, &text, widget->button.name, strlen(widget->button.name));

	}
	
	if (widget->button.tick) {
		hovering = widget->button.tick((bsgfx_ButtonParams) {
			.menu = menu,
			.widget = widget,
			.widget_position = position,
			.hovering = hovering,
		});
	}

	bs_mat4 transform = bs_transform(bs_v3AddZ(position, 1), BS_QUAT_IDENTITY, bs_v3(size.x, size.y, 0.0));
	bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR], bs_m4x3(transform), bs_v4(0, 0, 1, 1), 0, 0, hovering ? widget->button.selected_material : widget->button.material);

	//if (widget->prefer_x) {
	if (widget->advance_flags & BSGFX_WIDGET_ADVANCE_RIGHT) {
		position.y += size.y;
		position.x += size.x + 1.0;
	}

	static float hover_time;
	const float hover_time_threshhold = 0.5;
	if (hovering && widget->button.hint) {
		hover_time += bs_deltaTime();
		//if (hover_time > hover_time_threshhold)
		//	bsgfx_instanceHint(menu->text_subtype, bs_v3(cursor.x, cursor.y, 100.0), widget->button.hint);
	}
	else
		hover_time = 0.0;

	*out_width = size;

	return hovering;
}

/**
 Color Picker
 */
void bsgfx_renderColorPickers() {
	bs_beginComment("Color picker");
	bs_Pipeline* pipeline;
	bs_PipelineHash hash;

	 /**
	  Color picker
	  */
	hash = (bs_PipelineHash) {
		.shaders = {
			$vs_bsgfx_quad_rounded_instanced(),
			$fs_bsgfx_color_picker(),
		},
		BSGFX_TRANSPARENT_OPTIONS,
				.stencil_back = {
			.fail_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,
			.pass_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,
			.depth_fail_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,
			.compare_op = BS_COMPARE_OP_ALWAYS,
			.compare_mask = 0xFF,
			.write_mask = 0xFF,
		},
		.stencil_front = {
			.fail_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,
			.pass_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,
			.depth_fail_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,
			.compare_op = BS_COMPARE_OP_ALWAYS,
			.compare_mask = 0xFF,
			.write_mask = 0xFF,
		}
	};

	pipeline = bs_pipeline(&hash);

	bs_pushConstant(pipeline, 0, sizeof(_poser_->screen_camera.result), &_poser_->screen_camera.result);
	bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_COLOR_PICKER], pipeline);

	 /**
	  Hue
	  */
	hash = (bs_PipelineHash) {
		.shaders = {
			$vs_bsgfx_quad_rounded_instanced(),
			$fs_bsgfx_hue(),
		},
		BSGFX_TRANSPARENT_OPTIONS,
	};

	pipeline = bs_pipeline(&hash);

	bs_pushConstant(pipeline, 0, sizeof(_poser_->screen_camera.result), &_poser_->screen_camera.result);
	bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_COLOR_PICKER_HUE], pipeline);


	 /**
	  Alpha
	  */
	hash = (bs_PipelineHash){
		.shaders = {
			$vs_bsgfx_quad_rounded_instanced(),
			$fs_bsgfx_alpha(),
		},
		BSGFX_TRANSPARENT_OPTIONS,
	};

	pipeline = bs_pipeline(&hash);

	bs_pushConstant(pipeline, 0, sizeof(_poser_->screen_camera.result), &_poser_->screen_camera.result);
	bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_COLOR_PICKER_ALPHA], pipeline);

	bs_endComment();
}

typedef enum {
	BSGFX_ORIGIN_LEFT,
	BSGFX_ORIGIN_CENTER,
} bsgfx_OriginType;

static void bsgfx_instanceUIElement(bsgfx_AtlasCache* cache, bsgfx_OriginType origin, bs_vec3 position, int material_id) {
	if (origin == BSGFX_ORIGIN_CENTER) {
		position.x -= cache->size.x / 2.0;
		position.y -= cache->size.x / 2.0;
	}
	bs_mat4x3 icon_matrix = bsgfx_roundedMatrix(position, bs_v3V2(cache->size, 0.0), bs_v4V1(0.0));
	bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_STENCIL], icon_matrix, cache->coords, 0, 0, material_id);
}

static bool bsgfx_instanceColorPicker(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, bs_vec3 position, bs_vec2* out_size) {
	int border_size = (bool)widget->color.outline_material_id + (bool)widget->color.shadow_material_id;
	position.y -= widget->color.dimensions.y;
	//position->y -= border_size;

	bs_vec2 cursor = bs_windowCursorPosition();
	bs_vec2 hue_dimensions = bs_v2(widget->color.hue_width, widget->color.dimensions.y);
	bs_vec2 alpha_dimensions = bs_v2(widget->color.hue_width, widget->color.dimensions.y);
	bs_vec4 border_radii = bs_v4V1(widget->color.border_radius);
	bs_vec3 hue_position = bs_v3(position.x + widget->color.dimensions.x + widget->color.padding, position.y, position.z);
	bs_vec3 alpha_position = bs_v3(hue_position.x + hue_dimensions.x + widget->color.padding, position.y, position.z);
	
	float z = 1.0;

	 /**
	  Background
	  */
	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3AddZ(position, z),
		widget->color.dimensions, 
		border_radii,
		0,
		widget->color.outline_material_id,
		widget->color.shadow_material_id);

	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3AddZ(hue_position, z),
		hue_dimensions,
		border_radii,
		0,
		widget->color.outline_material_id,
		widget->color.shadow_material_id);

	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3AddZ(alpha_position, z),
		alpha_dimensions,
		border_radii,
		0,
		widget->color.outline_material_id,
		widget->color.shadow_material_id);
	z += BSGFX_BACKGROUND_Z_COUNT;

	 /**
	  Color picker
	  */
	bsgfx_instanceQuad(
		_bsgfx_subtypes_[BSGFX_SUBTYPE_COLOR_PICKER],
		bsgfx_roundedMatrix(bs_v3AddZ(position, 10), bs_v3V2(widget->color.dimensions, 0.0), border_radii),
		bs_v4(0, 0, 1, 1),
		0, 0, widget->color.material_id);
	
	bsgfx_instanceQuad(
		_bsgfx_subtypes_[BSGFX_SUBTYPE_COLOR_PICKER_HUE],
		bsgfx_roundedMatrix(bs_v3AddZ(hue_position, z), bs_v3V2(hue_dimensions, 0.0), border_radii),
		bs_v4(0, 0, 1, 1),
		0, 0, $white_material()->id);

	bsgfx_instanceQuad(
		_bsgfx_subtypes_[BSGFX_SUBTYPE_COLOR_PICKER_ALPHA],
		bsgfx_roundedMatrix(bs_v3AddZ(alpha_position, z), bs_v3V2(alpha_dimensions, 0.0), border_radii),
		bs_v4(0, 0, alpha_dimensions.x / alpha_dimensions.y, 1.0),
		0, 0, $white_material()->id);

	//int ui_bar = bs_queryAtlas(atlas, "ui_bar");

	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3(hue_position.x, hue_position.y + widget->color.hsva->x / 360.0 * hue_dimensions.y, position.z + 30),
		bs_v2(hue_dimensions.x, 1.0),
		bs_v4V1(0),
		widget->color.background_material_id,
		widget->color.outline_material_id, 
		0);

	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3(alpha_position.x, alpha_position.y + widget->color.hsva->w * alpha_dimensions.y, position.z + 30),
		bs_v2(hue_dimensions.x, 1.0),
		bs_v4V1(0),
		widget->color.background_material_id,
		widget->color.outline_material_id,
		0);

	bool hovering_picker = !already_hovering && bs_rectangleVsPoint(position.xy, widget->color.dimensions, cursor);
	bool hovering_hue = !already_hovering && bs_rectangleVsPoint(hue_position.xy, hue_dimensions, cursor);
	bool hovering_alpha = !already_hovering && bs_rectangleVsPoint(alpha_position.xy, alpha_dimensions, cursor);
	bool changed = false;

	static bool dragging_picker;
	static bool dragging_hue;
	static bool dragging_alpha;

	if ((hovering_picker || dragging_picker) && !dragging_hue && !dragging_alpha) {
		bs_vec2 relative = bs_v2(cursor.x - position.x, cursor.y - position.y);
		bs_v2Div(&relative, &widget->color.dimensions, &relative);
		relative = bs_v2(bs_clamp(relative.x, 0.0, 1.0), bs_clamp(relative.y, 0.0, 1.0));

		if (bs_leftClick()) {
			widget->color.hsva->y = relative.x;
			widget->color.hsva->z = relative.y;
			changed = true;
			dragging_picker = true;
		}
	}

	if (!bs_leftClick())
		dragging_picker = false;

	if ((hovering_hue || dragging_hue) && !dragging_picker && !dragging_alpha) {
		bs_vec2 relative = bs_v2(cursor.x - hue_position.x, cursor.y - hue_position.y);
		bs_v2Div(&relative, &hue_dimensions, &relative);

		if (bs_leftClick()) {
			widget->color.hsva->x = bs_clamp(relative.y * 360.0, 0.0, 360.0);
			changed = true;
			dragging_hue = true;
		}
	}

	if (!bs_leftClick())
		dragging_hue = false;

	if ((hovering_alpha || dragging_alpha) && !dragging_hue && !dragging_picker) {
		bs_vec2 relative = bs_v2(cursor.x - alpha_position.x, cursor.y - alpha_position.y);
		bs_v2Div(&relative, &alpha_dimensions, &relative);

		if (bs_leftClick()) {
			widget->color.hsva->w = bs_clamp(relative.y, 0.0, 1.0);
			changed = true;
			dragging_alpha = true;
		}
	}

	if (!bs_leftClick())
		dragging_alpha = false;

	if (changed) {
		if (widget->color.on_change_color) {
			bs_RGBA rgba = bs_rgb(widget->color.hsva->xyz);
			rgba.a = widget->color.hsva->w * 255.0;
			widget->color.on_change_color(widget, rgba);
		}
	}

	// circumcise
	//bsgfx_instanceUIElement($BSMOD_ATLAS_UI_circle(), BSGFX_ORIGIN_CENTER, bs_v3(
	//	position.x + widget->color.hsva->y * widget->color.dimensions.x,
	//	position.y + widget->color.hsva->z * widget->color.dimensions.y,
	//	position.z + 30), $white_material()->id);

	if (widget->advance_flags & BSGFX_WIDGET_ADVANCE_RIGHT) {
		bs_Font* font = widget->font ? widget->font : menu->font;
	//	position->y -= font->height + widget->name_padding;
		position.x += widget->color.dimensions.x + hue_dimensions.x + alpha_dimensions.x + widget->color.padding * 3;
	}

	return hovering_picker || hovering_hue;
}

/**
 Input
 */
static bs_U64 bsgfx_selected_input = 0;

static inline void bsgfx_findClosestX(bs_Font* font, bs_String* input, int* select_position, float* select_draw_position_x, float target) {
	float result_draw_position_x = 0.0;

	for (; *select_position < input->len; (*select_position)++) {
		char c = input->value[*select_position];

		// next line is shorter than the input line
		if (c == '\n')
			break;

		float size = c == ' ' ? font->spacing : bs_atlasSize(font->atlas, font->table[c]).x;
		float next_draw_position_x = result_draw_position_x + size * BSGFX_PIXEL_SCALE;

		// found
		if (result_draw_position_x <= target && next_draw_position_x > target)
			break;

		result_draw_position_x = next_draw_position_x;
	}

	if (select_draw_position_x)
		*select_draw_position_x = result_draw_position_x;
}

static inline void bsgfx_inputCursorPosition(bs_Font* font, bs_String* input, float relative_x, float relative_y, int* position) {
	float current = 0.0;

	// find y
	int target_row = relative_y / bsgfx_textHeight();
	int actual_row = 0;
	if (target_row != 0) {
		for (*position = 0; *position < input->len; (*position)++) {
			char c = input->value[*position];
			if (c == '\n') {
				if (++actual_row == target_row) {
					(*position)++;
					break;
				}
			}
		}
	}

	// find x
//	for (*position = 0; *position < input->len; (*position)++) {
//		int index = font->table[input->value[*position]];
//		if (index >= font->atlas->count)
//			index = 0;
//
//		float size = bs_atlasSize(font->atlas, index).x;
//		current += size * BSGFX_PIXEL_SCALE + BSGFX_PIXEL_SCALE;
//		if (current > relative_x)
//			break;
//	}

	bsgfx_findClosestX(font, input, position, NULL, relative_x);
}

static inline void bsgfx_findPreviousLine(bs_Font* font, bs_String* input, int* select_position_out, float* select_draw_position_x, float* select_draw_position_y) {
	int select_position = (*select_position_out) - 1;
	float target = *select_draw_position_x;

	// go to previous row
	bool hit = false; // bad
	for (; select_position > 0; select_position--) {
		char c = input->value[select_position];
		if (c == '\n') {
			select_position--;
			hit = true;
			break;
		}
	}

	if (select_position < 0 || !hit)
		return;

	// go to start of previous row
	for (; select_position > 0; select_position--) {
		char c = input->value[select_position];
		if (c == '\n') {
			select_position++;
			break;
		}
	}

	bsgfx_findClosestX(font, input, &select_position, select_draw_position_x, *select_draw_position_x);

	*select_position_out = select_position;
	*select_draw_position_y += bsgfx_textHeight(&select_position);
}

static inline void bsgfx_findNextLine(bs_Font* font, bs_String* input, int* select_position_out, float* select_draw_position_x, float* select_draw_position_y) {
	int select_position = *select_position_out;

	// go to next row
	for (; select_position >= 0 && select_position < input->len; select_position++) {
		char c = input->value[select_position];
		if (c == '\n') {
			select_position++;
			break;
		}
	}

	if (select_position >= input->len)
		return;

	bsgfx_findClosestX(font, input, &select_position, select_draw_position_x, *select_draw_position_x);

	*select_position_out = select_position;
	*select_draw_position_y -= bsgfx_textHeight(&select_position);
}

static void bsgfx_deserializeInputValue(bsgfx_Widget* widget, bs_String* string) {
	switch (widget->input.type) {
	case BSGFX_INPUT_DOUBLE: *widget->input.as_double = bs_toDouble(string->value); break;
	case BSGFX_INPUT_FLOAT: *widget->input.as_float = bs_toDouble(string->value); break;
	case BSGFX_INPUT_ULONG: *widget->input.as_ulong = bs_toULong(string->value); break;
	case BSGFX_INPUT_LONG: *widget->input.as_long = bs_toLong(string->value); break;
	case BSGFX_INPUT_UINT: *widget->input.as_uint = bs_toULong(string->value); break;
	case BSGFX_INPUT_INT: *widget->input.as_uint = bs_toLong(string->value); break;
	case BSGFX_INPUT_CHAR: *widget->input.as_uint = bs_toLong(string->value); break;
	case BSGFX_INPUT_UCHAR: *widget->input.as_uint = bs_toULong(string->value); break;
	case BSGFX_INPUT_STRING:
		return *widget->input.as_string = bs_string(*widget->input.as_string, string->value, string->len);
	default:
		bsi_throwBasiliskF(BSX_INVALID_TYPE, "Input type %d", widget->input.type);
		return NULL;
	}
}

static bs_String* bsgfx_serializeInputValue(bsgfx_Widget* widget, bs_String* string) {

	switch (widget->input.type) {
	case BSGFX_INPUT_DOUBLE: return bs_stringF(string, "%.4f", (float)*widget->input.as_double);
	case BSGFX_INPUT_FLOAT: return bs_stringF(string, "%.4f", (float)*widget->input.as_float);
	case BSGFX_INPUT_ULONG: return bs_stringF(string, "%lld", (long long)*widget->input.as_ulong);
	case BSGFX_INPUT_LONG: return bs_stringF(string, "%lld", (long long)*widget->input.as_long);
	case BSGFX_INPUT_UINT: return bs_stringF(string, "%d", (int)*widget->input.as_uint);
	case BSGFX_INPUT_INT: return bs_stringF(string, "%d", (int)*widget->input.as_int);
	case BSGFX_INPUT_CHAR: return bs_stringF(string, "%d", (int)*widget->input.as_char);
	case BSGFX_INPUT_UCHAR: return bs_stringF(string, "%d", (int)*widget->input.as_uchar);
	case BSGFX_INPUT_STRING: 
		return *widget->input.as_string ? bs_string(string, (*widget->input.as_string)->value, (*widget->input.as_string)->len) : string;
	default:
		bsi_throwBasiliskF(BSX_INVALID_TYPE, "Input type %d", widget->input.type);
		return NULL;
	}
}

static bool bsgfx_instanceInput(
	bsgfx_Menu* menu,
	bsgfx_Widget* widget,
	bool already_hovering,
	bs_vec3 position,
	bs_vec2* out_width,
	const char* alphabet)
{
	bs_vec2 cursor = bs_windowCursorPosition();

	bs_Font* font = widget->font ? widget->font : menu->font;

	static bs_String* string;
	if (!string)
		string = bs_string(string, "", 0);

	string = bsgfx_serializeInputValue(widget, string);

	const int outline_width = widget->input.outline_material_id != 0;
	float height = widget->input.dimensions.y + outline_width * 2;

	if (widget->align_height == 0.0)
		widget->align_height = height;

	position.y -= (widget->align_height + height) / 2.0;

	if (widget->input.active)
		bsgfx_selected_input = widget->input.hash;

	// input background
	bool hovering = !already_hovering && bs_rectangleVsPoint(position.xy, widget->input.dimensions, cursor);

	static char blinking_underscore = ' ';
	const float blink_every = 0.6;
	static float blink_timer = 0.0;

	// click selecting (WIP!!!)
	int start = 0;
	int end = 0;

	if (widget->input.select_position && widget->input.select_size) {
		int select_position = *widget->input.select_position;
		int select_size = *widget->input.select_size;

		if (bs_leftClickOnce()) {
			if (hovering) {
				if (bsgfx_selected_input != widget->input.hash) {
					bsgfx_selected_input = widget->input.hash;
					//select_position = value->len;
					//select_size = -value->len;
					//first_select = true;
				}
				//else {
				bsgfx_inputCursorPosition(font, string, cursor.x - position.x, widget->input.dimensions.y - (cursor.y - position.y), &select_position);
				select_size = 0;
				//}
			}
			else
				bsgfx_selected_input = select_position = select_size = 0;
		}

		if (hovering)
			bs_setCursor(BS_CURSOR_TEXT);
		else
			bs_setCursor(BS_CURSOR_DEFAULT);

		int x_dir = 0, y_dir = 0;
		if (bs_keyHeld(BS_KEY_LEFT)) x_dir = -1;
		if (bs_keyHeld(BS_KEY_RIGHT)) x_dir = 1;
		if (bs_keyHeld(BS_KEY_UP)) y_dir = -1;
		if (bs_keyHeld(BS_KEY_DOWN)) y_dir = 1;

		if (!bs_keyDown(BS_KEY_LEFT_SHIFT)) {
			if (x_dir != 0)
				select_size = 0;
		}

		if (x_dir != 0 || y_dir != 0) {
			select_position += x_dir;
			blinking_underscore = '|';
			blink_timer = 0.0;
		}

		// assert(value);

		// wouldnt be surprised if the select_size calculation breaks here
		if (select_position < 0 || (select_position + select_size) < 0) {
			select_size += *widget->input.select_position;
			select_position = 0;
		}
		else if (select_position > string->len || (select_position + select_size) > string->len) {
			select_position = string->len;
		}
		else if (bs_keyDown(BS_KEY_LEFT_SHIFT)) {
			if (bs_keyHeld(BS_KEY_LEFT))
				select_size++;
			else if (bs_keyHeld(BS_KEY_RIGHT))
				select_size--;
		}

		int row_position = 0;
		int row_index = 0;
		for (int i = 0, reset = 0; i < select_position; i++, reset++) {
			if (string->value[i] == '\n') {
				row_position += reset + 1;
				row_index++;
				reset = -1; // will increment to 0
			}
		}

		if (bs_keyHeld(BS_KEY_END)) {
			blink_timer = 0.0;
			blinking_underscore = '|';

			int start = select_position + select_size;

			for (; select_position < string->len; select_position++) {
				if (string->value[select_position] == '\n') {
					break;
				}
			}

			if (bs_keyDown(BS_KEY_LEFT_SHIFT))
				select_size = start - select_position;
			else
				select_size = 0;
		}
		else if (bs_keyHeld(BS_KEY_HOME)) {
			blink_timer = 0.0;
			blinking_underscore = '|';
			if (bs_keyDown(BS_KEY_LEFT_SHIFT))
				select_size += select_position - row_position;
			else
				select_size = 0;
			select_position = row_position;
		}
		else if (bs_keyDown(BS_KEY_LEFT_CONTROL) && bs_keyHeld(BS_KEY_A)) {
			blink_timer = 0.0;
			blinking_underscore = '|';
			select_position = 0;
			select_size = string->len;
		}

		char* row = string->value + row_position;
		float select_draw_position_x = bs_textDimensions(font, row, select_position - row_position).x;
		float select_draw_position_y = widget->input.dimensions.y - bsgfx_textHeight() - (row_index * bsgfx_textHeight());
		if (y_dir != 0) {
			if (bs_keyDown(BS_KEY_LEFT_SHIFT)) {
				int select_size_end = select_position;
				float select_size_draw_position_x = select_draw_position_x;
				float select_size_draw_position_y = select_draw_position_y;

				if (y_dir > 0)
					bsgfx_findNextLine(font, string, &select_position, &select_size_draw_position_x, &select_size_draw_position_y);
				if (y_dir < 0)
					bsgfx_findPreviousLine(font, string, &select_position, &select_size_draw_position_x, &select_size_draw_position_y);

				select_size += (select_size_end - select_position);
			}
			else {
				select_size = 0;
				if (y_dir > 0)
					bsgfx_findNextLine(font, string, &select_position, &select_draw_position_x, &select_draw_position_y);
				if (y_dir < 0)
					bsgfx_findPreviousLine(font, string, &select_position, &select_draw_position_x, &select_draw_position_y);
			}
		}

		if (bsgfx_selected_input == widget->input.hash) {
			blink_timer += bs_deltaTime();

#define BSGFX_REPLACE_CHARS { \
		bs_except(BSX_OUT_OF_BOUNDS); \
		int size = select_size == 0 ? -1 : select_size;\
		if (size > 0)\
			bs_removeCharRange(string, select_position, size);\
		else {\
			bs_removeCharRange(string, select_position + size, -size);\
			select_position += size;\
		}\
		select_size = 0; \
		bs_caught(); }

			if (bs_keyHeld(BS_KEY_BACKSPACE) && string->len > 0) {
				blink_timer = 0.0;
				blinking_underscore = '|';

				BSGFX_REPLACE_CHARS;

				bsgfx_deserializeInputValue(widget, string);
			}
			else if (bs_keyHeld(BS_KEY_ENTER) && widget->input.new_line_on_enter) {
				blink_timer = 0.0;
				blinking_underscore = '|';

				if (select_size != 0)
					BSGFX_REPLACE_CHARS;

				string = bs_insertChar(string, select_position++, '\n');
				bsgfx_deserializeInputValue(widget, string);
			}
			else if (bs_keyHeld(BS_KEY_SPACE)) {
				blink_timer = 0.0;
				blinking_underscore = '|';

				if (select_size != 0)
					BSGFX_REPLACE_CHARS;

				string = bs_insertChar(string, select_position++, ' ');
				bsgfx_deserializeInputValue(widget, string);
			}
			else {
				for (int i = 0; font->alphabet[i] != '\0'; i++) {
					if (bs_charDown(font->alphabet[i])) {
						if (select_size != 0)
							BSGFX_REPLACE_CHARS;

						blink_timer = 0.0;
						blinking_underscore = '|';

						string = bs_insertChar(string, select_position++, font->alphabet[i]);
						bsgfx_deserializeInputValue(widget, string);

						break;
					}
				}
			}
		}

		start = select_position;
		end = select_position + select_size;
		if (start > end) {
			int temp = start;
			start = end;
			end = temp;
		}

		*widget->input.select_position = select_position;
		*widget->input.select_size = select_size;

		if (bsgfx_selected_input == widget->input.hash && blinking_underscore == '|') {
			char c[2] = { blinking_underscore, '\0' };
			bsgfx_textInstance(menu->text_subtype, font, &(bsgfx_Text) {
				.position = { position.x + select_draw_position_x, position.y + select_draw_position_y - font->min_y_shift, position.z + 4, 1 },
				.scale = 16.0, // todo font scale
				.material_id = $black_material()->id,
			}, c, 1);
		}
	}

	if (bsgfx_selected_input == widget->input.hash && blink_timer > blink_every) {
		blinking_underscore = blinking_underscore == '|' ? ' ' : '|';
		blink_timer = 0.0;
	}

	bsgfx_Text text = {
		.colors = {
			BS_BLACK, BS_RED, BS_GREEN, BS_YELLOW, BS_BLUE, BS_MAGENTA, BS_CYAN, BS_WHITE
		},
		.position = { position.x, position.y - font->min_y_shift, position.z + 4, 1 },
		.scale = 16.0, // todo font scale
		.select_start = start,
		.select_end = end,
	};

	if (string->len == 0 && widget->input.placeholder_text) {
		text.material_id = widget->input.placeholder_text_material_id;
		bsgfx_textInstance(menu->text_subtype, font, &text, widget->input.placeholder_text, strlen(widget->input.placeholder_text));
	}
	else {
		text.material_id = widget->material_id;
		bsgfx_textInstance(menu->text_subtype, font, &text, string->value, BS_MIN(string->len, 1024));
	}

	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		bs_v3AddZ(position, 3),
		widget->input.dimensions,
		bs_v4V1(menu->border_radius),
		widget->input.outline_material_id,
		widget->input.background_outline_material_id,
		widget->input.background_shadow_material_id);

	//	if (active && bs_keyDownOnce(BS_KEY_ENTER) && widget->input.action) {
	//		widget->input.action(widget);
	//		bsgfx_selected_input = 0;
	//	}

	*out_width = bs_v2(widget->input.dimensions.x, height);


	return hovering;
}

/**
 Grid
 */
static bs_U64 bsgfx_selected_grid = 0;
static bool bsgfx_instanceGrid(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, bs_vec3 position, bs_vec2* out_width) {
	bs_vec2 cursor = bs_windowCursorPosition();

	int columns = floor(widget->grid.total_size.x / widget->grid.size.x);
	int rows = floor(widget->grid.total_size.y / widget->grid.size.y);
	rows++;

	const float wrap_after_x = columns * widget->grid.size.x;
	const float wrap_after_y = rows * widget->grid.size.y;

	const int advance = widget->grid.scrollbar.substeps_count <= 0 ? 1 : widget->grid.scrollbar.substeps_count;
	const float scroll_offset = 1.0 / (float)advance;


	int max_scroll = 0;
	if (widget->grid.scrollbar.value) {
		bool hovering_area = bs_rectangleVsPoint(bs_v2AddY(position.xy, -widget->grid.total_size.y), widget->grid.total_size, cursor);
		if (hovering_area) {
			*widget->grid.scrollbar.value -= bs_scroll();
			if (*widget->grid.scrollbar.value < 0)
				*widget->grid.scrollbar.value = 0;
		}

		int total_rows = (int)ceil(widget->grid.count / (float)columns);
		int max_rows_count = total_rows - rows;
		if (max_rows_count < 0)
			max_rows_count = 0;

		max_scroll = max_rows_count * advance + 1;

		if (*widget->grid.scrollbar.value > max_scroll)
			*widget->grid.scrollbar.value = max_scroll;
	}
	bs_vec3 start = position;

	position.y -= widget->grid.size.y;
	float start_y = position.y;

	bs_vec2 grid_position = position.xy;

	bool hovering = false;
	if (!widget->grid.always_active) {
		hovering = !already_hovering && bs_rectangleVsPoint(position.xy, widget->grid.size, cursor);
		widget->grid.action(widget, (bsgfx_GridParams) {
			.start = position.xy, 
			.position = &grid_position, 
			.index = -1
		});
		grid_position.x += widget->grid.size.x;

	//	if (hovering && bs_leftClickOnce())
	//		bsgfx_selected_grid = bsgfx_selected_grid == id ? 0 : id;
	//	if (bsgfx_selected_grid != id)
	//		return hovering;
	}

	int i = 0;
	int draw_count = 0;

	if (widget->grid.scrollbar.value)
		grid_position.y += (float)(*widget->grid.scrollbar.value % advance) * widget->grid.size.y * scroll_offset;

	for (int row = 0; row < rows; row++) {
		for (int col = 0; col < columns; col++, i++) {
		//	bool hovering_grid = !already_hovering && bs_rectangleVsPoint(grid_position, widget->grid.size, cursor);
		//	if (hovering_grid)
		//		hovering = true;

			int index = i + columns * (int)((float)*widget->grid.scrollbar.value * scroll_offset);
			if (index >= widget->grid.count)
				goto end;

			bool should_advance = widget->grid.action(widget, (bsgfx_GridParams) {
				.start = position.xy, 
				.position = &grid_position, 
				.index = index
			});

			if (should_advance) {
				grid_position.x += widget->grid.size.x + widget->grid.padding;
				draw_count++;
			}

			if (grid_position.y < (start_y - wrap_after_y))
				goto end;


			if (grid_position.x > (position.x + (columns - 1) * widget->grid.size.x)) {
			//if (draw_count > 0 && draw_count % columns == 0) {
				grid_position.x = position.x;
				grid_position.y -= widget->grid.size.y + widget->grid.padding;
			}
		}
	}

end:
	if (widget->grid.scrollbar.value) {
		bs_vec3 scrollbar_position = bs_v3(start.x + widget->grid.total_size.x - widget->grid.scrollbar.padding, start.y - widget->grid.scrollbar.padding, start.z);
		bsgfx_instanceScrollbar(
			&widget->grid.scrollbar,
			scrollbar_position,
			max_scroll,
			*widget->grid.scrollbar.value,
			widget->grid.total_size.y - widget->grid.scrollbar.padding * 2);
	}

	*out_width = widget->grid.total_size;

	position.y -= rows * widget->grid.size.y;
	return hovering;
}

/**
 Model
 */
static bool bsgfx_instanceModel(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, bs_vec3 position, bs_vec2* out_size) {
	bs_throwBasilisk(BSX_NOT_IMPLEMENTED);
	/*
	bs_vec2 cursor = bs_windowCursorPosition();

	bs_U64 hash = bs_stringHash(widget->model.name);
	bs_Model* model = bs_queryHash(widget->model.name, NULL, hash, 0).model;
	bs_Mesh* mesh = widget->model.mesh_name ? bs_queryMesh(model, widget->model.mesh_name) : NULL;

	bs_Aabb aabb = mesh ? mesh->aabb : model->aabb;

	bs_vec2 dim = { 256, 256 };
	bs_vec2 sc = position->xy;

	float width = aabb.max.x - aabb.min.x;
	float height = aabb.max.y - aabb.min.y;
	float depth = aabb.max.z - aabb.min.z;

	sc = bs_v2Div(sc, bs_v2MulV1(BSGFX_TILE_SIZE, 2));
	sc.x += (width / BSGFX_PIXEL_SCALE);
	sc.y -= (height / BSGFX_PIXEL_SCALE) * 2.0 * 2.0;

	static float diff = 0.0;
	static float start = 0.0;

	if (bs_leftClickOnce())
		start = cursor.x;

	if (bs_leftClick()) {
		diff += cursor.x - start;
		start = cursor.x;
	}

	const float rotation_factor = 0.5;

	bs_vec4 rotation = bs_qFromDegreesV3(0.0, 180 + diff * rotation_factor, 0.0);
	bs_mat4 transform = BS_MAT4_IDENTITY;
	bs_vec3 p = bs_v3(sc.x + width, sc.y - depth * BS_SIN_45, 0);

	bs_translateP(&transform, &p);
	bs_rotateP(&transform, &rotation);

	bs_vec3 offset = { -width / 2.0, 0.0, depth / 2.0 };
	bs_translateP(&transform, &offset);

	if (widget->model.instance)
		widget->model.instance(&transform);
		*/
	return false;
}

/**
 Table
 */
static bool bsgfx_instanceTable(bsgfx_Menu* menu, bsgfx_Widget* widget, bool already_hovering, bs_vec3 position, bs_vec2* out_size) {
	bs_vec2 cursor = bs_windowCursorPosition();

	bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
	bs_Font* font = widget->font ? widget->font : menu->font;
	int white = bs_queryAtlas(atlas, "white");
	bs_vec4 coords = bs_atlasCoordinates(atlas, white, 0);
//	int atlas_any_hi_res_subtype = bsgfx_querySubtypeNull(BSGFX_INSTANCE_TYPE_QUAD_LEGACY, BSGFX_QUAD_LEGACY_KEY_ATLAS_ANY_HI_RES);

	bsgfx_Text text = {
		.position = { position.x, position.y - bsgfx_textHeight() - BSGFX_PIXEL_SCALE, position.z + 2, 1 },
		.scale = BSGFX_PIXEL_SCALE,
	};

	int actual_unit_size = 0;
	for (int i = 0; i < widget->table.fields->count; i++) {
		bsgfx_TableField* field = bs_fetchUnit(widget->table.fields, i);
		switch (field->type) {
		case BSGFX_TABLE_FIELD_FLOAT: actual_unit_size += field->count * sizeof(float); break;
		case BSGFX_TABLE_FIELD_INTEGER: actual_unit_size += field->count * sizeof(int); break;
		default:
			bs_throwBasilisk(BSX_INVALID_TYPE);
			return false;
		}
	}

	if (actual_unit_size != widget->table.unit_size) {
		bs_throwBasilisk(BSX_MISMATCH);
		return false;
	}

	static bs_String* string;
	if (!string)
		string = bs_stringAlloc(NULL, 128);

	unsigned char* field_data = widget->table.data;
	for (int i = 0; i < widget->table.fields->count; i++) {
		bsgfx_TableField* field = bs_fetchUnit(widget->table.fields, i);
		bs_vec2 text_dim = bsgfx_textInstanceF(font, &text, " %s |", field->name);
		text_dim.y += bsgfx_textHeight();

	//	bsgfx_quadLegacyInstance(atlas_any_hi_res_subtype,
	//		bs_v4V2(text.position.xy, position->z + 1, 1),
	//		text_dim,
	//		coords,
	//		i % 2 == 0 ? BS_BLACK : bs_rgba(150, 150, 150, 255),
	//		0);

		bsgfx_Text row_text = {
			.position = text.position,
			.scale = BSGFX_PIXEL_SCALE,
			.max_length = text_dim.x,
		};

		// string builder
		int count = BS_MIN(widget->table.max_count, widget->table.count);
		int field_size = 0;
		for (int j = 0; j < count; j++) {
			string = bs_emptyString(string);
			field_size = 0;
			for (int k = 0; k < field->count; k++) {
				unsigned char* value_data = field_data + k * actual_unit_size;

				switch (field->type) {
				case BSGFX_TABLE_FIELD_FLOAT:
					field_size += sizeof(float);
					bs_appendStringF(string, "%.1f"); break;
				case BSGFX_TABLE_FIELD_INTEGER:
					field_size += sizeof(int);
					bs_appendStringF(string, "%d"); break;
				}

				if (k != (field->count - 1))
					bs_appendString(string, ", ", 2);
			}

			string = bs_appendChar(string, '\n');

			bsgfx_textInstance(menu->text_subtype, font, &row_text, string->value, string->len);
			row_text.position.y -= bsgfx_textHeight();
		}

		field_data += field->count * field_size;

		text.position.x += text_dim.x;
	}

	//*out_size = bs_v2(, bsgfx_textHeight()).

	return false;
}



  /*==============================================================================
   * Menu System
   =============================================================================*/

static bool bsgfx_instanceDebugSettingsMenu(bsgfx_Menu* menu, bsgfx_TitleBar* title_bar, bs_vec3 position, bs_vec2 dimensions, int border_id, float title_bar_height) {
	bs_vec2 cursor = bs_windowCursorPosition();

	// int atlas_any_hi_res_subtype = bsgfx_querySubtypeNull(BSGFX_INSTANCE_TYPE_QUAD_LEGACY, BSGFX_QUAD_LEGACY_KEY_ATLAS_ANY_HI_RES);

	position.y -= dimensions.y;
	bool hovering = bsgfx_rectangleVsPointExpand(position.xy, dimensions, cursor, 1.0);

	position.z -= 10; // add some z space so widgets can use negative offsets, maybe change to something else
	bs_mat4x3 transform = bsgfx_roundedMatrix(position, bs_v3(dimensions.x, dimensions.y, 0.0), bs_v4V1(0));
	if (menu->background_material_id_1)
		bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_CORNER_GRADIENT], transform, bs_v4(0, 0, 1, 1), 0, menu->background_material_id_1, menu->background_material_id_0);
	else
		bsgfx_instanceQuad(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR], transform, bs_v4(0, 0, 1, 1), 0, 0, menu->background_material_id_0);

	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		position,
		bs_v2AddY(dimensions, title_bar ? title_bar_height : 0.0),
		bs_v4V1(menu->border_radius),
		0,
		menu->outline_material_id,
		menu->shadow_material_id);

	return hovering;
}

static void bsgfx_instanceMenuTabs(bsgfx_Menu* menu, bsgfx_MenuTabBar* tab_bar) {
	const int x_indent = 0.0;
	bs_Font* font = menu->font;
	bs_vec3 position = menu->position;
	bs_vec3 scale = bs_v3(64.0, tab_bar->height, 1.0);

	position.x += x_indent;
	scale.x -= x_indent;
	position.z -= 10;
	bs_vec3 start = position;

	bs_vec2 close_dimensions = bs_textDimensions(font, " [x]", 4);


	position.z -= BSGFX_BACKGROUND_Z_COUNT;
	bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
		position,
		bs_v2(menu->untextured.dimensions.x, scale.y),
		bs_v4(menu->border_radius, menu->border_radius, menu->border_radius, menu->border_radius),
		tab_bar->material_id,
		tab_bar->outline_material_id,
		tab_bar->shadow_material_id);

	position.z += BSGFX_BACKGROUND_Z_COUNT;
	for (int i = 0; i < tab_bar->tabs_count; i++) {
		position.x +=  4;
		bsgfx_MenuTab* tab = tab_bar->tabs + i;

		bs_vec2 name_dimensions = bsgfx_textInstance(menu->text_subtype, font, &(bsgfx_Text) {
			.position = {
				position.x + 4.0 + (tab->icon_cache ? tab->icon_cache->size.x : 0.0),
				position.y + tab_bar->height / 2.0 - 4, // todo idk why 4 will figure out when i try another font
				position.z + 1,
				1,
			},
			.scale = 16.0,
			.max_length = menu->untextured.dimensions.x / (float)tab_bar->tabs_count
		}, tab->name, strlen(tab->name));

		name_dimensions.x += 16.0;
		scale.x = name_dimensions.x + x_indent;

		if (tab->icon_cache) {
			bs_vec3 tab_icon_offset = bs_v3(0.0, (tab_bar->height - tab->icon_cache->size.y) / 2.0, 10.0);
			bs_vec3 tab_icon_position;
			bs_v3Add(&position, &tab_icon_offset, &tab_icon_position);
			bsgfx_instanceQuad(
				_bsgfx_subtypes_[BSGFX_SUBTYPE_UI],
				bsgfx_roundedMatrix(
					tab_icon_position,
					bs_v3V2(tab->icon_cache->size, 0.0), 
					bs_v4(0, 0, 0, 0)
				),
				tab->icon_cache->coords,
				0, 0, tab->icon_material_id); 

			scale.x += tab->icon_cache->size.x;
		}

		if (bs_rectangleVsPoint(position.xy, scale.xy, bs_windowCursorPosition())) {
			_poser_->menu_blocked = true;
			if (bs_leftClickOnce())
				*tab_bar->active_tab = i;
		}

		if (*tab_bar->active_tab == i) {
			bsgfx_instanceBackground(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI_COLOR],
				position,
				scale.xy,
				bs_v4(0, 0, menu->border_radius, menu->border_radius),
				tab->material_id,
				tab->outline_material_id,
				tab->shadow_material_id);
		}

		if (tab->icon_cache)
			position.x += tab->icon_cache->size.x;
		position.x += name_dimensions.x + 4;
		/*
		bsgfx_quadInstance(_bsgfx_subtypes_[BSGFX_SUBTYPE_UI],
			bsgfx_roundedMatrix(
				bs_v3AddZ(position, 1.0),
				bs_v3V2(title_bar_close->size, 0.0),
				bs_v4V1(0)
			),
			title_bar_close->coords,
			0, 0, $white_material()->id);
		*/
		position.y = start.y;
	}
}

static void bsgfx_instanceTitleBar(bsgfx_Menu* menu, bsgfx_TitleBar* title_bar, bs_vec3 position, bs_vec2 dimensions) {
	bs_Font* font = title_bar->font ? title_bar->font : menu->font;
	bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
	int texture = bs_queryAtlas(atlas, "white");
	bs_vec4 coords = bs_atlasCoordinates(atlas, texture, 0);

	const int bar_padding = 8;
	 /**
	  Title bar
	  */
	bsgfx_instanceQuad(
		_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS_HI],
		bsgfx_roundedMatrix(position, bs_v3(dimensions.x, dimensions.y, 1.0), bs_v4(0, 0, 0, 0)),
		coords, 
		0, 0, title_bar->material_id);

	int offset = 0;

	 /**
	  Title bar name
	  */
	if (title_bar->name) {
		bsgfx_textInstance(menu->text_subtype, font, &(bsgfx_Text) {
			.position = bs_v4(position.x + bar_padding, position.y + bar_padding, position.z + 1, 1),
			.scale = 16.0,
		}, title_bar->name, strlen(title_bar->name));
	}

	 /**
	  Buttons
	  */
	const int button_padding = 24;
	const float button_height = 16;
	const float outline_width = 1.0;
	/*

	struct {
		int id;
		bs_vec4 radii;
		void(*action)();
	} buttons[] = {
		{.id = bs_queryAtlas(atlas_ui, "title_bar_close"), .action = title_bar->on_close,.radii = {0, title_bar->border_radius, title_bar->border_radius, 0}},
		{.id = bs_queryAtlas(atlas_ui, "title_bar_maximize"), .action = title_bar->on_maximize },
		{.id = bs_queryAtlas(atlas_ui, "title_bar_minimize"), .action = title_bar->on_minimize, .radii = { title_bar->border_radius, 0, 0, title_bar->border_radius } },
	};

	position.x -= bar_padding / 2;
	position.y += bar_padding / 2;

	bs_vec3 button_position;

	const int count = (!title_bar->on_close && !title_bar->on_maximize && !title_bar->on_minimize) ? 0 : 3;
	for (int i = 0; i < count; i++) {
		bs_vec2 size = bs_v2MulV1(bs_atlasSize(atlas_ui, buttons[i].id), 1.0);

		offset += size.x + button_padding + outline_width;

		bs_vec3 button_size = bs_v3(size.x + button_padding, button_height, 1.0);
		button_position = bs_v3AddX(position, dimensions.x);
		button_position.x -= offset;
		button_position.z += 1;

		bool hovering = buttons[i].action && bs_rectangleVsPoint(button_position.xy, button_size.xy, bs_windowCursorPosition());
		bsgfx_instanceQuad(
			_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS_HI],
			bsgfx_roundedMatrix(button_position, button_size, buttons[i].radii),
			coords,
			0, 0, buttons[i].action ? (hovering ? title_bar->button_hovering_material_id : title_bar->button_material_id) : title_bar->button_unavailable_material_id);

		if (hovering && bs_leftClickOnce())
			buttons[i].action();

		bs_vec3 icon_size = bs_v3(size.x, size.y, 1.0);
		bs_vec3 icon_position = bs_v3AddX(position, dimensions.x - offset);
		icon_position.x += button_padding / 2;
		icon_position.y += button_height / 4.0;
		icon_position.z += 2;

		bsgfx_instanceQuad(
			_bsgfx_subtypes_[BSGFX_SUBTYPE_UI],
			bsgfx_roundedMatrix(icon_position, icon_size, bs_v4(0, 0, 0, 0)),
			bs_atlasCoordinates(atlas_ui, buttons[i].id, 0),
			0, 0, title_bar->button_icon_material_id);
	}
	*/

	if (title_bar->button_shadow_material_id) {
		bs_vec3 outline_size = bs_v3(offset - outline_width, button_height, 1.0);
		bs_vec3 outline_position = bs_v3AddX(position, dimensions.x);
		outline_position.y -= outline_width;
		outline_position.x -= offset;
		outline_position.z++;

		bsgfx_instanceQuad(
			_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS_HI],
			bsgfx_roundedMatrix(outline_position, outline_size, bs_v4V1(title_bar->border_radius)),
			coords,
			0, 0, title_bar->button_shadow_material_id);
	}
}

bool bsgfx_instanceWidgets(bsgfx_Menu menu, bsgfx_TitleBar* title_bar, bsgfx_MenuTabBar* tab_bar) {
	if (tab_bar)
		bsgfx_instanceMenuTabs(&menu, tab_bar);

	const int title_bar_height = 24.0;
	if (title_bar) {
		bs_vec3 title_bar_offset = bs_v3(0, 0, 1);
		bs_vec3 title_bar_position;
		bs_v3Add(&menu.position, &title_bar_offset, &title_bar_position);
		bsgfx_instanceTitleBar(&menu, title_bar, title_bar_position, bs_v2(menu.untextured.dimensions.x, title_bar_height));
	}

	bool hovering = menu.blocked;
	bool hovering_widgets = false;

	bs_vec3 menu_start_position = menu.position, menu_position = menu.position;
	float lowest_y = menu_start_position.y;
	bs_vec2 cursor = bs_windowCursorPosition();
	 
	for (int i = 0; i < menu.widgets_count; i++) {
		bsgfx_Widget* widget = menu.widgets + i;
		if (widget->disabled)
			continue;

		bs_v3Add(&menu.position, &widget->offset, &menu.position);
		float start_x = menu.position.x;
		float start_y = menu.position.y;
		float start_z = menu.position.z;

		bs_vec2 widget_size = { 0 };

		//menu.position.y -= widget->align_height;

		bool hovering_widget = false;
		switch (widget->type) {
		case BSGFX_WIDGET_STRING: hovering_widget = bsgfx_instanceString(&menu, widget, hovering, menu.position, &widget_size); break;
		case BSGFX_WIDGET_BUTTON: hovering_widget = bsgfx_instanceButton(&menu, widget, hovering, menu.position, &widget_size); break;
		case BSGFX_WIDGET_INPUT: hovering_widget = bsgfx_instanceInput(&menu, widget, hovering, menu.position, &widget_size,
			"!#$%&'()*+,-./abcdefghijklmnopqrstuvwxyz0123456789:;[\\]_{|}~"); break;
		case BSGFX_WIDGET_GRID: hovering_widget = bsgfx_instanceGrid(&menu, widget, hovering, menu.position, &widget_size); break;
		case BSGFX_WIDGET_URL: hovering_widget = bsgfx_instanceUrl(&menu, widget, hovering, i, menu.position, &widget_size); break;
		case BSGFX_WIDGET_RANGE: hovering_widget = bsgfx_instanceRange(&menu, widget, hovering, i, menu.position, &widget_size); break;
		case BSGFX_WIDGET_SLIDER: hovering_widget = bsgfx_instanceSlider(&menu, widget, hovering, i, menu.position, &widget_size); break;
		case BSGFX_WIDGET_ICON: hovering_widget = bsgfx_instanceIcon(&menu, widget, hovering, i, menu.position, &widget_size); break;
		case BSGFX_WIDGET_LIST: hovering_widget = bsgfx_instanceList(&menu, widget, hovering, i, menu.position, &widget_size); break;
		case BSGFX_WIDGET_COLOR: hovering_widget = bsgfx_instanceColorPicker(&menu, widget, hovering, menu.position, &widget_size); break;
		case BSGFX_WIDGET_TABLE: hovering_widget = bsgfx_instanceTable(&menu, widget, hovering, menu.position, &widget_size); break;
		case BSGFX_WIDGET_MODEL: hovering_widget = bsgfx_instanceModel(&menu, widget, hovering, menu.position, &widget_size); break;
		case BSGFX_WIDGET_BACKGROUND: 
			widget_size = widget->background.size;
			bsgfx_instanceBackground(widget->background.subtype,
				bs_v3AddY(menu.position, -widget->background.size.y),
				widget->background.size,
				bs_v4V1(widget->background.border_radius),
				widget->background.material_id,
				widget->background.outline_material_id,
				widget->background.shadow_material_id);
			break;
		}

		if (hovering_widget)
			hovering_widgets = true;

		if (!hovering)
			hovering = hovering_widget;

		widget_size.y = BS_MAX(widget_size.y, widget->align_height);

		if (!(widget->advance_flags & BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_X))
			menu.position.x = menu.position.x - widget->offset.x;
		if (!(widget->advance_flags & BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Y))
			menu.position.y = menu.position.y - widget->offset.y;
		if (!(widget->advance_flags & BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Z))
			menu.position.z = menu.position.z - widget->offset.z;

		menu.position.z = start_z;

		if (widget->advance_flags & BSGFX_WIDGET_ADVANCE_RIGHT)
			menu.position.x += widget_size.x + menu.spacing;
		if (widget->advance_flags & BSGFX_WIDGET_ADVANCE_DOWN)
			menu.position.y -= widget_size.y + menu.spacing;
		if (widget->advance_flags & BSGFX_WIDGET_ADVANCE_RESET_X)
			menu.position.x = menu_start_position.x;
	}
	// dimensions->y = start.y - position.y;

	bool hovering_menu = bsgfx_instanceDebugSettingsMenu(&menu, title_bar, menu_start_position, menu.untextured.dimensions, menu.untextured.border_id, title_bar_height);

	if (hovering_widgets || hovering_menu)
		_poser_->menu_blocked = true;
	return hovering_widgets || hovering_menu;
}
