#ifndef BSGFX_H
#define BSGFX_H

#include <basilisk-core.h>

#include <../bsgfx_contracts.h>
#include <bssteam_controller.h>
#include <bsgfx_instance.h>

static inline bs_vec2 bs_windowCursorPosition() {
	return bs_v2Mul(bs_cursorPosition(), bs_v2Iv2(bs_resolution()));
}

#define BSGFX_CONTENT_PATH "content/bsgfx"

#define BSGFX_PARSE_FORMAT(format, data, len)                         \
    do {                                                           \
        va_list args;                                              \
        va_start(args, format);                                    \
        len = vsnprintf(NULL, 0, format, args);                    \
        va_end(args);                                              \
        if (len >= (sizeof(data) / sizeof(*data))) {               \
            bs_throwBasiliskF(BSX_OUT_OF_BOUNDS,                   \
                "Format %s has a limit of %d characters",          \
                format, sizeof(data) - 1);                         \
        }                                                          \
        va_start(args, format);                                    \
        vsprintf(data, format, args);                              \
        va_end(args);                                              \
    } while (0)

#define BSGFX_MAX_NUMBER_PLAYERS (8)
#define BSGFX_PIXEL_SCALE (4.0)
#define BSGFX_FIXED_TICK (1.0 / 40.0)

#define BSGFX_TITLE_BAR_COLOR (bs_RGBA) { 255, 255, 255, 200 }

#define BSGFX_SCREENSHOT_DIMENSIONS bs_iv2(1920 / 4, 1080 / 4)

#define BSGFX_MOTD							\
	"poser\n"							\
	"pre-pre-alpha\n\n"					\
	"thx for playing\n"					\
	"special thx malteSWE!"

#define BSGFX_SHADOW_COLOR ((bs_RGBA) { 50, 50, 60, 150 })

#define BSGFX_RGBA(r, g, b, a) ((bs_vec4) { (float)r / 255.0, (float)g / 255.0, (float)b / 255.0, (float)a / 255.0 })
#define BSGFX_WHITE BSGFX_RGBA(255, 255, 255, 255)

#define BSGFX_TILE_SIZE ((bs_vec2) { 32, 32 })
//#define BSGFX_TILE_SIZE.x (34.0)
//#define BSGFX_TILE_SIZE.y (BSGFX_TILE_SIZE.x * 0.7071)
// #define BSGFX_TILE_SIZE.y ((int)(BSGFX_TILE_SIZE.x * BS_SIN_45)) // generates artifacts, tiles can be unaligned

#define BSGFX_TEXTURE_PATH "resources/textures/"
#define BSGFX_MODEL_SUBTYPE(X) \
	X(BSGFX_MODEL_SUBTYPE_DEFAULT) \
	X(BSGFX_MODEL_SUBTYPE_MIRRORED) \
	X(BSGFX_MODEL_SUBTYPE_COUNT)

enum {
	BSGFX_MODEL_SUBTYPE(BSGFX_ENUM_GEN)
};

typedef enum {
    BSGFX_GRAPHICS_PIPE = (1 << 0),
    BSGFX_COMPUTE_PIPE = (1 << 1),
    BSGFX_SHADOW_PIPE = (1 << 2),
    BSGFX_BONE_PIPE = (1 << 3),
    BSGFX_MESH_PIPE = (1 << 4),
	BSGFX_RENDER_LINES = (1 << 5),
	BSGFX_RENDER_TILES = (1 << 6),
	BSGFX_ALL_PIPES = 0xFFFFFFFF,
} bsgfx_Pipe;

typedef struct {
	bs_U64 handle;

	struct {
		bs_U64 action_set_handle;
		bs_Joystick move;
		bs_Button interact;
		bs_Button jump;
		bs_Button prepare_skate;
	} ingame;

	struct {
		bs_U64 action_set_handle;
		bs_Button left, right, up, down;
		bs_Button cancel, affirm;
	} menu;
} bsgfx_Controller;

extern bsgfx_Controller _bsgfx_ctrl;
extern bsgfx_Controller _bsgfx_ctrl_last;
extern bsgfx_Controller _bsgfx_ctrl_last_fixed;

extern bs_mat4* _bsgfx_shader_joints;
extern bs_U32 _bsgfx_num_shader_joints;

typedef struct bsgfx_Settings bsgfx_Settings;
extern struct bsgfx_Settings {
	bool draw_spheres;
	bool draw_lines;
	bool cull_backfaces;
	bool draw_tiles;
	bool draw_primitives;
	bool draw_foliage;
	bool draw_sprites;
	bool draw_shadows;
	bool draw_spawners;

	float master_volume;
	bool music;
} _bsgfx_settings;

extern struct Poser {
	int day;
	int hour;
	int minute;
	int second;
	float time;
	bs_vec3 sun_direction;

    struct {
        bs_mat4 proj;
        bs_mat4 view;
        bs_mat4 result; // proj * view
     //   bs_vec3 position;
     //   bs_vec3 direction;
    } camera;

	struct { // 2d camera synced with 3d
		bs_mat4 proj;
		bs_mat4 view;
		bs_mat4 result;
		bs_vec2 position;
	} world_camera;

	struct {
		bs_mat4 result;
	} screen_camera;

	float zoom;

	bool instances_ticked;
	bs_Buffer* instance_buffers[BSGFX_INSTANCE_TYPE_COUNT];

	struct{
		int joint_offset;
	} members[BSGFX_MAX_NUMBER_PLAYERS];
	int member_count;
	//bsgfx_LobbyData* player_data[BSGFX_MAX_NUMBER_PLAYERS];

	bs_List sweep_collisions; // bsgfx_SweepCollision
	bool menu_blocked;
	bool menu_open;
}* _poser;

typedef void(__stdcall* PFN_void)(); // TODO y no prefix?
typedef bs_Queue*(__stdcall* PFN_bsmod_onQueue)();
typedef bool(__stdcall* PFN_bsmod_isSelected)(enum bsgfx_TypeId type, int id);
typedef bool(__stdcall* PFN_bs_hoveringGrid)();
typedef void(__stdcall* PFN_bsmod_onMap)(enum bsgfx_TypeId type_id, int id);
typedef void(__stdcall* PFN_bsmod_onCreateSubtypes)(bs_Range range);

#define BSGFX_FOREACH_PROC(X) \
    X(PFN_void, bsmod_onIni) \
    X(PFN_void, bsmod_onLateIni) \
    X(PFN_void, bsmod_onLoad) \
    X(PFN_void, bsmod_copyHoveringDataToBuffer) \
    X(PFN_bsmod_onCreateSubtypes, bsmod_onCreateQuadSubtypes) \
    X(PFN_bsmod_onQueue, bsmod_onQueue) \
    X(PFN_void, bsmod_onGfxRender) \
    X(PFN_bsmod_isSelected, bsmod_isSelected) \
    X(PFN_bsmod_onMap, bsmod_onMap) \
    X(PFN_void, bsmod_onTrack) \

#define BSGFX_STRUCT_GEN(TYPE, FUNC, ...) TYPE FUNC;
#define BSGFX_COUNT_GEN(TYPE, FUNC, ...) +1

typedef struct bsgfx_Procedures bsgfx_Procedures;
// NOTE: Don't add anything else in here
extern struct bsgfx_Procedures {
	BSGFX_FOREACH_PROC(BSGFX_STRUCT_GEN)
} _bsgfx_procs;

typedef struct {
	PFN_void tick;
	PFN_void fixedTick;
	PFN_void render;
	PFN_void ini;
	PFN_void loadScene;
} bsgfx_Callbacks;

typedef struct {
	const char* name;
} bsgfx_Application;

extern bsgfx_Application _bsgfx_app;
extern bs_Model* _bsgfx_prefab_model;
extern bsgfx_Callbacks _bsgfx_callbacks;
extern HINSTANCE _bsgfx_bsmod_dll;
extern bs_String* _bsgfx_variadic;
extern int _bsgfx_package;

BSGFXAPI bs_Model* bsgfx_prefabModel();

BSGFXAPI int bsgfx_package();

BSGFXAPI void 
bsgfx_ini(
	const char* name, 
	bs_U32 width, 
	bs_U32 height, 
	int argc, 
	char* argv[]);

BSGFXAPI HINSTANCE bsgfx_bsmodDll();

BSGFXAPI void bsgfx_openMenu();
BSGFXAPI void bsgfx_closeMenu();

BSGFXAPI void bsgfx_checkGFSDK(bs_U32 result);
BSGFXAPI void bsgfx_logGFSDK(bs_U32 result);

BSGFXAPI bsgfx_Application* bsgfx_app();
BSGFXAPI bsgfx_Callbacks* bsgfx_callbacks();
BSGFXAPI bsgfx_Settings* bsgfx_settings();
BSGFXAPI struct Poser* poser();
BSGFXAPI void
bsgfx_setCamera(
	const bs_mat4* proj,
	const bs_mat4* view);


static inline bs_vec2 bsgfx_worldToScreen(bs_vec3 position, bs_mat4 camera, bs_vec2 resolution) {
	bs_vec4 clip = bs_m4MulV4(camera, bs_v4V3(position, 1.0));

//	assert(clip.w != 0.0);
	if (clip.w == 0.0) return bs_v2(0.0, 0.0);

	bs_vec3 ndc = bs_v3DivV1(clip.xyz, clip.w);

	bs_vec2 screen = {
		.x = (ndc.x * 0.5 + 0.5) * resolution.x,
		.y = (ndc.y * 0.5 + 0.5) * resolution.y,
	};

	return screen;
}


#endif