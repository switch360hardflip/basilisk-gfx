#ifndef BSGFX_UI_H
#define BSGFX_UI_H

#include <bs_types.h>
#include <bsgfx_scene.h>
#include <types/bsgfx_type.h>
#include <bsgfx.h>

#define BSGFX_LIST_ROW_DIMENSIONS BS_V2(320, 6.0 * BSGFX_PIXEL_SCALE)
#define BSGFX_BACKGROUND_Z_COUNT 3

typedef struct bsgfx_ButtonParams bsgfx_ButtonParams;
typedef struct bsgfx_MenuTabBar bsgfx_MenuTabBar;
typedef struct bsgfx_MenuTab bsgfx_MenuTab;
typedef struct bsgfx_Menu bsgfx_Menu;
typedef struct bsgfx_TitleBar bsgfx_TitleBar;

typedef enum {
	BSGFX_WIDGET_UNDEFINED,
	BSGFX_WIDGET_STRING,
	BSGFX_WIDGET_BUTTON,
	BSGFX_WIDGET_INPUT,
	BSGFX_WIDGET_GRID,
	BSGFX_WIDGET_URL,
	BSGFX_WIDGET_RANGE,
	BSGFX_WIDGET_ICON,
	BSGFX_WIDGET_LIST,
	BSGFX_WIDGET_COLOR,
	BSGFX_WIDGET_SLIDER,
	BSGFX_WIDGET_MODEL,
	BSGFX_WIDGET_TABLE, // unfinished
	BSGFX_WIDGET_BACKGROUND, // scuffed
} bsgfx_WidgetType;

typedef struct bsgfx_Widget bsgfx_Widget;
typedef struct {
	bsgfx_Widget* widget;
	bs_vec3* position;
	int id;
} bsgfx_ForeachVisibleRowParams;

struct bsgfx_ButtonParams {
	bsgfx_Menu* menu;
	bsgfx_Widget* widget;
	bs_vec3 widget_position;
	bool hovering;
};

typedef struct {
	const char* name;
	int count;
	enum {
		BSGFX_TABLE_FIELD_INTEGER,
		BSGFX_TABLE_FIELD_FLOAT,
	} type;
} bsgfx_TableField;

typedef struct {
	int* value;
	float width;
	float border_radius;
	int substeps_count;
	int padding;

	int material;
	int outline_material;
	int background_material;

	int button_icon_material;
	int button_outline_material;
	int button_background_material;
} bsgfx_Scrollbar;

typedef struct {
	bs_vec2 start;
	bs_vec2* position;
	int index;
} bsgfx_GridParams;

typedef enum {
	BSGFX_INPUT_DOUBLE,
	BSGFX_INPUT_FLOAT,
	BSGFX_INPUT_ULONG,
	BSGFX_INPUT_LONG,
	BSGFX_INPUT_UINT,
	BSGFX_INPUT_INT,
	BSGFX_INPUT_CHAR,
	BSGFX_INPUT_UCHAR,
	BSGFX_INPUT_STRING,
} bsgfx_InputType;

typedef enum {
	BSGFX_WIDGET_ADVANCE_DOWN = (1 << 0),
	BSGFX_WIDGET_ADVANCE_RIGHT = (1 << 1),
	BSGFX_WIDGET_ADVANCE_RESET_X = (1 << 2),
	BSGFX_WIDGET_ADVANCE_RESET_Y = (1 << 3),
	BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_X = (1 << 4),
	BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Y = (1 << 5),
	BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Z = (1 << 6),
	BSGFX_WIDGET_ADVANCE_APPLY_OFFSET = (BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_X | BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Y | BSGFX_WIDGET_ADVANCE_APPLY_OFFSET_Z),
} bsgfx_WidgetAdvanceType;

struct bsgfx_Widget {
	int indent;
	int id;

	int material_id;
	const char* hint;

	bsgfx_WidgetType type;

	bool disabled;
	bsgfx_WidgetAdvanceType advance_flags;

	void* params;

	bs_vec3 offset;

	float align_height;
	bs_Font* font;

	union {
		struct {
			char* value;
			void(*on_hover)(struct bsgfx_Widget*);
		} string;

		struct {
			bsgfx_InputType type;
			bool active;
			bool new_line_on_enter;
			const char* placeholder_text;
			int placeholder_text_material_id;
			int outline_material_id;
			int background_outline_material_id;
			int background_shadow_material_id;
			bs_vec2 dimensions;
			bs_U64 hash;
			int* select_position;
			int* select_size;
			union {
				double* as_double;
				float* as_float;
				long long* as_long;
				unsigned long long* as_ulong;
				int* as_int;
				int* as_uint;
				char* as_char;
				unsigned char* as_uchar;
				bs_String** as_string;

				void* as_void;
			};
		} input;

		struct {
			bool always_active;
			int selected;
			int count;
			bs_vec2 size;
			bs_vec2 total_size;
			float padding;
			bool(*action)(bsgfx_Widget*, bsgfx_GridParams);
			bsgfx_Scrollbar scrollbar;
		} grid;

		struct {
			char* path;
		} url;

		struct {
			int material;
			int selected_material;
			bs_vec2 size;
			bool(*tick)(bsgfx_ButtonParams);
			// hacky
			struct {
				int* active_index;
				int this_index;
			} active_override;

			const char* name;
			const char* hint;

			enum {
				BSGFX_BUTTON_ALIGN_CENTER,
				BSGFX_BUTTON_ALIGN_LEFT,
			} alignment;
		} button;

		struct {
			int* value;
			int min, max;
			void(*on_change)(struct bsgfx_Widget*);
		} range;

		struct {
			float* value;
			float min, max;
			float width;
			bool integer;
		} slider;

		struct {
			enum {
				BSGFX_ICON_ATLAS,
				BSGFX_ICON_TILE,
				BSGFX_ICON_MATERIAL,
				BSGFX_ICON_64,
				BSGFX_ICON_256,
				BSGFX_ICON_512,
			} type;

			bs_vec2 scale;
			const char* name;
			const char* background_name;
			const char* background_selected_name;
			int frame;

			void(*hover)(struct bsgfx_Widget*);
			int material_id;
			float border_radius;
			int outline_material_id;
			bs_Atlas* atlas;
			int atlas_subtype;

			bool flipped;
			bool mirrored;
		} icon;

		struct {
			bs_vec4* hsva;
			bs_vec2 dimensions;
			float hue_width;
			float border_radius;
			int padding;
			int material_id;
			int background_material_id;
			int shadow_material_id;
			int outline_material_id;
			void(*on_change_color)(struct bsgfx_Widget*, bs_RGBA);
		} color;

		struct {
			int max;
			int vk_offset;
			int count;
			int* scroll;
			char* (*foreach_visible_row)(bsgfx_ForeachVisibleRowParams params);
		} list;

		struct {
			const char* name;
			const char* mesh_name;
			void (*instance)(const bs_mat4* transform);
		} model;

		struct {
			const bs_List* fields;
			unsigned char* data;
			int count, unit_size;
			int max_count;
		} table;

		struct {
			bs_vec2 size;
			float border_radius;
			int subtype;
			int material_id;
			int outline_material_id;
			int shadow_material_id;
		} background;
	};
};

static inline int bsgfx_copyWidgets(bsgfx_Widget* source, bsgfx_Widget* destination, size_t size) {
	int count = size / sizeof(bsgfx_Widget);

	memcpy(destination, source, size);
	memset(destination + count, 0, sizeof(bsgfx_Widget));

	return count;
}

struct bsgfx_MenuTab {
	const char* name;
	bs_U64 hash;
	bsgfx_AtlasCache* icon_cache;
	int icon_material_id;
	int material_id;
	int outline_material_id;
	int shadow_material_id;
};

struct bsgfx_MenuTabBar {
	bsgfx_MenuTab* tabs;
	int* active_tab;
	int tabs_count;

	float height;

	int material_id;
	int outline_material_id;
	int shadow_material_id;
};

struct bsgfx_Menu {
	bs_vec3 position;
	bsgfx_Widget* widgets;
	int widgets_count;

	float spacing;
	bool blocked;

	int background_material_id_0;
	int background_material_id_1; // for gradients

	struct {
		bs_vec2 dimensions;
		int border_id;
		bool auto_scale_width;
	} untextured;

	bs_Font* font;
	float border_radius;
	int shadow_material_id;
	int outline_material_id;

	int text_subtype;
};

struct bsgfx_TitleBar {
	const char* name;
	bs_Font* font;
	int material_id;
	int button_unavailable_material_id;
	int button_material_id;
	int button_hovering_material_id;
	int button_icon_material_id;
	int button_shadow_material_id;
	int button_selected_shadow_material_id;

	void(*on_close)();
	void(*on_maximize)();
	void(*on_minimize)();

	float border_radius;
};

BSGFXAPI bool
bsgfx_instanceWidgets(
	bsgfx_Menu menu,
	bsgfx_TitleBar* title_bar,
	bsgfx_MenuTabBar* tab_bar);

BSGFXAPI void
bsgfx_renderColorPickers();

#endif