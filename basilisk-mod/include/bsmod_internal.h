#ifndef BSMOD_H
#define BSMOD_H

#include <bs_types.h>
#include <types/bsgfx_type.h>

#define BSMODAPI _declspec(dllexport)

#define BSMOD_CONTENT_PATH "content/bsmod"
#define BSMOD_CONFIG_PATH "project/bsmod_config.json"

enum {
	BSMOD_SUBTYPE_MATERIAL_ICON,
	BSMOD_SUBTYPE_PRIMITIVE_ICON,
	BSMOD_SUBTYPE_PREFAB_ICON,
	BSMOD_SUBTYPE_TILE_ICON,
	BSMOD_SUBTYPE_SPHERE_HIGH_QUALITY,
	BSMOD_SUBTYPE_FONT_CONSOLAS,
	BSMOD_SUBTYPE_BILLBOARD,

	BSMOD_SUBTYPE_COUNT,
};
BSMODAPI extern int _bsmod_subtypes[BSMOD_SUBTYPE_COUNT];

BSMODAPI void bsmod_copyHoveringDataToBuffer();

BSMODAPI void bsmod_onIni();
BSMODAPI void bsmod_onLateIni();
BSMODAPI void bsmod_onCreateQuadSubtypes(bs_Range range);
BSMODAPI void bsmod_onLoad();
BSMODAPI void bsmod_bindAtlases();
BSMODAPI void bsmod_onGfxRender();
BSMODAPI void bsmod_onPostRender();
BSMODAPI void bsmod_onTick();
BSMODAPI void bsmod_onMap(bsgfx_TypeId type_id, int id);
BSMODAPI void bsmod_onTrack();

typedef struct {
	char* path;
	char* package;
	char* prefix;
	union {
		struct {
			bool compile_references;
		};
	};
} bsmod_TrackParams;

BSMODAPI void bsmod_packBindings();
BSMODAPI void bsmod_onCompileShader(bsmod_TrackParams params);
BSMODAPI void bsmod_onLoadTTF(bsmod_TrackParams params);
BSMODAPI void bsmod_onConvertBMFont(bsmod_TrackParams params);
BSMODAPI void bsmod_onPackAtlas(bsmod_TrackParams params);
BSMODAPI void bsmod_onPackModels(bsmod_TrackParams params);
BSMODAPI void bsmod_onPackBinary(bsmod_TrackParams params);
BSMODAPI void bsmod_onPackTextureArray(bsmod_TrackParams params);

typedef enum {
	BSMOD_EDIT_UNDEFINED,
	BSMOD_EDIT_POSITION,
	BSMOD_EDIT_ROTATION,
	BSMOD_EDIT_SCALE,
} bsmod_EditType;

typedef enum {
	BSMOD_DRAGGING_UNDEFINED,
	BSMOD_DRAGGING_TILE,
	BSMOD_DRAGGING_ATLAS,
	BSMOD_DRAGGING_TEXTURE,
	BSMOD_DRAGGING_PREFAB,
	BSMOD_DRAGGING_MATERIAL,
	BSMOD_DRAGGING_PRIMITIVE,
	BSMOD_DRAGGING_ITEM,
} bsmod_DraggingType;

typedef struct {
	bs_ResourceType type;
	char* path;
} bsmod_QueueLoad;

BSMODAPI extern struct Bsmod {
	int package;
	int bsgfx_package;

	int selected_tile_primitive;
	int selected_tile_axis;

	struct {
		bs_RGBA color;
		bs_vec3 normal;
		bs_U32 index;
		bs_U32 flags;
		bs_vec3 position;
		int instance_type;
		int instance_id;
		int subtype;

		int closest_primitive;
		bs_vec3 closest_vertex;

		bool billboard;
		int primitive, prefab, tile, tile_axis;
	} hovering;

	bs_List selected_ids;
	bs_List selected_tiles;

	int editor_resource_type;
	int editor_resource_id;

	bs_String* foliage_density_input;
	bs_String* primitive_angle_input;

	bs_vec2 selected_tile_size;
	//bs_vec2 menu_offset;

	bool draw_menu;
	bool active;
	bool ui_blocked;

	int overlay;

	const char* (*grid_menu)(bs_List* widgets);
	void (*grid_menu_callback)(struct bsgfx_DebugMenuWidget*, int);
	void* grid_menu_params;

	int clicked_type;
	int clicked_id;
	bsgfx_TypeId selected_type;

	int axis;

	bs_I64 history; // current history timestamp

	struct {
		bool draw_hidden_primitives;
	} settings;

	// memset to 0 after every frame
	struct {
		int isolated_subtype;
		int isolated_id;

		bool screenshot;
		bool load_fonts, load_shaders, load_materials, load_textures, load_scripts;
		bool skip_mesh_index_write;
	} queue;

	bsgfx_Id dragging_object_id;
	int dragging_subtype;
	int dragging_id;

	bsmod_EditType edit_type;
	bsmod_EditType edit_type_old;
	bs_Json track_json;
	bs_Json bindings_json;

	HINSTANCE bsgfx;
	bs_String* variadic;

	bs_List queue_load;
} bsmod;

extern bs_Json _bsmod_config;

static inline const char* bsmod_applicationContentPath() {
	return bs_fetchJson(&_bsmod_config, BS_JSON_STRING, "application.content_path").as_string;
}

typedef void(__stdcall* PFN_void)();

#define BSMOD_FOREACH_PROC(X) \
    X(PFN_void, bsgfx_loadFonts) 

#define BSMOD_STRUCT_GEN(TYPE, FUNC, ...) TYPE FUNC;
#define BSMOD_COUNT_GEN(TYPE, FUNC, ...) +1

typedef struct bsmod_Procedures bsmod_Procedures;
// NOTE: Don't add anything else in here
extern struct bsmod_Procedures {
	BSMOD_FOREACH_PROC(BSMOD_STRUCT_GEN)
} _bsmod_procs;


bool bsmod_keyHeld(bs_U32 code);
bool bsmod_keyDown(bs_U32 code);
bool bsmod_keyDownOnce(bs_U32 code);
bool bsmod_keyUpOnce(bs_U32 code);
bool bsmod_charDown(unsigned char code);
bool bsmod_charDownOnce(unsigned char code);
bool bsmod_charUpOnce(unsigned char code);

bool bsmod_middleClick();
bool bsmod_middleClickOnce();
bool bsmod_middleClickUpOnce();
bool bsmod_leftClick();
bool bsmod_rightClick();
bool bsmod_rightClickOnce();
bool bsmod_leftClickOnce();
bool bsmod_rightClickUpOnce();
bool bsmod_leftClickUpOnce();


#define BSMOD_ENUM_GEN(ENUM) ENUM,
#define BSMOD_STRING_GEN(STRING) #STRING,



  /*==============================================================================
   * Images
   *============================================================================*/

#define BSMOD_IMAGE_IDS(X)									\
        X(BSMOD_IMAGE_DEPTH)                                \
        X(BSMOD_IMAGE_DEPTH_3D)                             \
        X(BSMOD_IMAGE_COLOR)

enum { BSMOD_IMAGE_IDS(BSMOD_ENUM_GEN) BSMOD_IMAGES_COUNT };
extern int _bsmod_images;
#define BSMOD_IMAGES _bsmod_images



  /*==============================================================================
   * Samplers
   *============================================================================*/

#define BSMOD_SAMPLER_IDS(X) \
	X(BSMOD_SAMPLER_PLACEHOLDER)

enum { BSMOD_SAMPLER_IDS(BSMOD_ENUM_GEN) BSMOD_SAMPLERS_COUNT };
extern int _bsmod_samplers;
#define BSMOD_SAMPLERS _bsmod_samplers



  /*==============================================================================
   * Buffers
   *============================================================================*/

#define BSMOD_BUFFER_IDS(X) \
	X(BSMOD_BUFFER_PLACEHOLDER)

enum { BSMOD_BUFFER_IDS(BSMOD_ENUM_GEN) BSMOD_BUFFERS_COUNT };
extern int _bsmod_buffers;
#define BSMOD_BUFFERS _bsmod_buffers



  /*==============================================================================
   * Batches
   *============================================================================*/

#define BSMOD_BATCH_IDS(X)									\
        X(BSMOD_BATCH_TILE)

enum { BSMOD_BATCH_IDS(BSMOD_ENUM_GEN) BSMOD_BATCHES_COUNT };
extern int _bsmod_batches;
#define BSMOD_BATCHES _bsmod_batches



  /*==============================================================================
   * Renderers
   *============================================================================*/

#define BSMOD_RENDERER_IDS(X)								\
        X(BSMOD_RENDERER)                                   \
        X(BSMOD_RENDERER_3D)

enum { BSMOD_RENDERER_IDS(BSMOD_ENUM_GEN) BSMOD_RENDERERS_COUNT };
extern int _bsmod_renderers;
#define BSMOD_RENDERERS _bsmod_renderers



  /*==============================================================================
   * Queues
   *============================================================================*/

#define BSMOD_QUEUE_IDS(X)									\
        X(BSMOD_QUEUE_GRAPHICS)                             \
        X(BSMOD_QUEUE_GRAPHICS_RASTERIZATION)

enum { BSMOD_QUEUE_IDS(BSMOD_ENUM_GEN) BSMOD_QUEUES_COUNT };
extern int _bsmod_queues;
#define BSMOD_QUEUES _bsmod_queues



  /*==============================================================================
   * Ray tracers
   *============================================================================*/

#define BSMOD_RAY_TRACER_IDS(X) \
	X(BSMOD_RAY_TRACER_PLACEHOLDER)

enum { BSMOD_RAY_TRACER_IDS(BSMOD_ENUM_GEN) BSMOD_RAY_TRACERS_COUNT };
extern int _bsmod_ray_tracers;
#define BSMOD_RAY_TRACERS _bsmod_ray_tracers



  /*==============================================================================
   * Atlases
   *============================================================================*/

#define BSMOD_ATLAS_IDS(X)                              \
        X(BSMOD_ATLAS_UI)                                   \
        X(BSMOD_ATLAS_MATERIAL_ICONS)                       \
        X(BSMOD_ATLAS_PRIMITIVE_ICONS)                      \
        X(BSMOD_ATLAS_PREFAB_ICONS)

enum { BSMOD_ATLAS_IDS(BSMOD_ENUM_GEN) BSMOD_ATLASES_COUNT };
extern int _bsmod_atlases;
#define BSMOD_ATLASES _bsmod_atlases



  /*==============================================================================
   * Fonts
   *============================================================================*/

#define BSMOD_FONT_IDS(X) \
	X(BSMOD_FONT_PLACEHOLDER)

enum { BSMOD_FONT_IDS(BSMOD_ENUM_GEN) BSMOD_FONTS_COUNT };
extern int _bsmod_fonts;
#define BSMOD_FONTS _bsmod_fonts

#endif
