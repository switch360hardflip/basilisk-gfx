
 /**
  MIT License
  
  Copyright (c) 2026 switch360hardflip <switch360hardflip@gmail.com>
  
  Permission is hereby granted, free of charge, to any person obtaining a copy
  of this software and associated documentation files (the "Software"), to deal
  in the Software without restriction, including without limitation the rights
  to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
  copies of the Software, and to permit persons to whom the Software is
  furnished to do so, subject to the following conditions:
  
  The above copyright notice and this permission notice shall be included in all
  copies or substantial portions of the Software.
  
  THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
  IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
  FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
  AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
  LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
  OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
  SOFTWARE.
  */ 

 /**
  This file was generated from basilisk-gfx.com

  It is not recommended to make changes to this file as it will be lost if
  the code is regenerated.
  */
        
#include <basilisk-core.h>
#include <../bsgfx_contracts.h>
#include <spawners/entity/bsgfx_entity.h>
#include <assert.h>
#include <stdarg.h>
#include <string.h>
#include <stdbool.h>

#ifndef BASILISK_GFX_H
#define BASILISK_GFX_H

typedef struct bsgfx_Scene bsgfx_Scene;
typedef struct bsgfx_Material bsgfx_Material;
typedef struct bsgfx_Collider bsgfx_Collider;
typedef struct bsgfx_SweepCollision bsgfx_SweepCollision;
typedef struct bsgfx_Animator bsgfx_Animator;
typedef struct bsgfx_AnimatorCallbacks bsgfx_AnimatorCallbacks;
typedef struct bsgfx_InstanceBuffer bsgfx_InstanceBuffer;
typedef struct bsgfx_MeshInstance bsgfx_MeshInstance;
typedef struct bsgfx_BoneInstance bsgfx_BoneInstance;
typedef struct bsgfx_QuadInstance bsgfx_QuadInstance;
typedef struct bsgfx_Text bsgfx_Text;
typedef struct bsgfx_Settings bsgfx_Settings;
typedef struct Poser Poser;
typedef struct bsgfx_Procedures bsgfx_Procedures;
typedef struct bsgfx_Callbacks bsgfx_Callbacks;
typedef struct bsgfx_Application bsgfx_Application;

typedef enum bsgfx_Subtype bsgfx_Subtype;
typedef enum bsgfx_ResourceType bsgfx_ResourceType;
typedef enum bsgfx_ImageId bsgfx_ImageId;
typedef enum bsgfx_SamplerId bsgfx_SamplerId;
typedef enum bsgfx_BufferId bsgfx_BufferId;
typedef enum bsgfx_BatchId bsgfx_BatchId;
typedef enum bsgfx_RendererId bsgfx_RendererId;
typedef enum bsgfx_QueueId bsgfx_QueueId;
typedef enum bsgfx_RayTracerId bsgfx_RayTracerId;
typedef enum bsgfx_AtlasId bsgfx_AtlasId;
typedef enum bsgfx_FontId bsgfx_FontId;
typedef enum bsgfx_MaterialCategory bsgfx_MaterialCategory;
typedef enum bsgfx_CollisionType bsgfx_CollisionType;
typedef enum bsgfx_AnimatorTypeBit bsgfx_AnimatorTypeBit;
typedef enum bsgfx_FontFlag bsgfx_FontFlag;
typedef enum bsgfx_ModelSubtype bsgfx_ModelSubtype;
typedef enum bsgfx_Pipe bsgfx_Pipe;

#define BSGFX_MODEL_PATH                                             \
    "resources/models/"

#define BSGFX_SHADER_PATH                                            \
    "resources/shaders/"

#define BSGFX_AUDIO_PATH                                             \
    "resources/audio/"

#define BSGFX_ENUM_GEN(ENUM)                                         \
    ENUM,

#define BSGFX_STRING_GEN(STRING)                                     \
    #STRING,

#define BSGFX_IMAGE_IDS(X)                                           \
    X(BSGFX_IMAGE_LO_RES_0_DEPTH)                                    \
    X(BSGFX_IMAGE_LO_RES_0_COLOR)                                    \
    X(BSGFX_IMAGE_LO_RES_0_NORMAL)                                   \
    X(BSGFX_IMAGE_LO_RES_0_POSITION)                                 \
    X(BSGFX_IMAGE_LO_RES_0_INDEX)                                    \
    X(BSGFX_IMAGE_LO_RES_0_FLAGS)                                    \
    X(BSGFX_IMAGE_LO_RES_0_VERTEX)                                   \
    X(BSGFX_IMAGE_LO_RES_1_COLOR)                                    \
    X(BSGFX_IMAGE_HI_RES_0_DEPTH)                                    \
    X(BSGFX_IMAGE_TILE)                                              \
    X(BSGFX_IMAGE_ANY)                                               \
    X(BSGFX_IMAGE_64)                                                \
    X(BSGFX_IMAGE_256)                                               \
    X(BSGFX_IMAGE_512)                                               \
    X(BSGFX_IMAGE_RAYTRACE_TEST)                                     \
    X(BSGFX_IMAGE_LEVEL_THUMBNAIL)                                   \
    X(BSGFX_IMAGE_MINIMAP)

#define BSGFX_SAMPLER_IDS(X)                                         \
    X(BSGFX_SAMPLER_NEAREST)                                         \
    X(BSGFX_SAMPLER_LINEAR)

#define BSGFX_BUFFER_IDS(X)                                          \
    X(BSGFX_BUFFER_JOINTS)                                           \
    X(BSGFX_BUFFER_INSTANCE_METADATA)                                \
    X(BSGFX_BUFFER_LO_RES_CURSOR_READS)                              \
    X(BSGFX_BUFFER_LO_RES_SCREENSHOT)                                \
    X(BSGFX_BUFFER_MATERIALS)                                        \
    X(BSGFX_BUFFER_MINIMAP)                                          \
    X(BSGFX_BUFFER_BINDING_TABLE)

#define BSGFX_BATCH_IDS(X)                                           \
    X(BSGFX_BATCH_VOLUME_COMPUTED)                                   \
    X(BSGFX_BATCH_PREFAB_SHADOWS)                                    \
    X(BSGFX_BATCH_VOLUME_INSTANCED)                                  \
    X(BSGFX_BATCH_VOLUME_SCREEN)                                     \
    X(BSGFX_BATCH_SCREEN)                                            \
    X(BSGFX_BATCH_LINE_INSTANCED)                                    \
    X(BSGFX_BATCH_POINT_INSTANCED)                                   \
    X(BSGFX_BATCH_SPHERE_INSTANCED)                                  \
    X(BSGFX_BATCH_BONE_INSTANCED)                                    \
    X(BSGFX_BATCH_MESH_INSTANCED)                                    \
    X(BSGFX_BATCH_MESH_TYPE_VOLUME_COMPUTED)                         \
    X(BSGFX_BATCH_FOLIAGE)                                           \
    X(BSGFX_BATCH_QUAD_INSTANCED)                                    \
    X(BSGFX_BATCH_TEXT_INSTANCED)                                    \
    X(BSGFX_BATCH_ITEMS)                                             \
    X(BSGFX_BATCH_PRIMITIVE_TILES)

#define BSGFX_RENDERER_IDS(X)                                        \
    X(BSGFX_RENDERER_LO_RES)                                         \
    X(BSGFX_RENDERER_HI_RES)

#define BSGFX_QUEUE_IDS(X)                                           \
    X(BSGFX_QUEUE_GRAPHICS)                                          \
    X(BSGFX_QUEUE_COMPUTE)

#define BSGFX_RAY_TRACER_IDS(X)                                      \
    X(BSGFX_RAY_TRACER_MAIN)

#define BSGFX_ATLAS_IDS(X)                                           \
    X(BSGFX_ATLAS_ANY)

#define BSGFX_FONT_IDS(X)                                            \
    X(BSGFX_FONT_STANDARD)                                           \
    X(BSGFX_FONT_SMALL)                                              \
    X(BSGFX_FONT_TEMPLE)                                             \
    X(BSGFX_FONT_CONSOLAS)                                           \
    X(BSGFX_FONT_ARIAL_16)                                           \
    X(BSGFX_FONT_ARIAL_24)                                           \
    X(BSGFX_FONT_ARIAL_32)                                           \
    X(BSGFX_FONT_ARIAL_48)

#define BSGFX_IMAGES                                                 \
    bsgfx_images()

#define BSGFX_SAMPLERS                                               \
    bsgfx_samplers()

#define BSGFX_BUFFERS                                                \
    bsgfx_buffers()

#define BSGFX_BATCHES                                                \
    bsgfx_batches()

#define BSGFX_RENDERERS                                              \
    bsgfx_renderers()

#define BSGFX_QUEUES                                                 \
    bsgfx_queues()

#define BSGFX_RAY_TRACERS                                            \
    bsgfx_rayTracers()

#define BSGFX_ATLASES                                                \
    bsgfx_atlases()

#define BSGFX_FONTS                                                  \
    bsgfx_fonts()

#define BSGFX_DEPENDENCY_ALWAYS                                      \
    BSGFX_IMAGE_64,                                                  \
    BSGFX_IMAGE_256,                                                 \
    BSGFX_IMAGE_512,                                                 \
    BSGFX_SAMPLER_LINEAR

#define BSGFX_DEPENDENCY_PLAYER                                      \
    BSGFX_BATCH_BONE_INSTANCED,                                      \
    BSGFX_BATCH_MESH_INSTANCED,                                      \
    BSGFX_BUFFER_JOINTS

#define BSGFX_DEPENDENCY_TILE                                        \
    BSGFX_IMAGE_TILE

#define BSGFX_DEPENDENCY_LO_RES                                      \
    BSGFX_QUEUE_GRAPHICS,                                            \
    BSGFX_IMAGE_LO_RES_0_DEPTH,                                      \
    BSGFX_IMAGE_LO_RES_0_COLOR,                                      \
    BSGFX_IMAGE_LO_RES_0_NORMAL,                                     \
    BSGFX_IMAGE_LO_RES_0_POSITION,                                   \
    BSGFX_IMAGE_LO_RES_0_INDEX,                                      \
    BSGFX_IMAGE_LO_RES_0_FLAGS,                                      \
    BSGFX_IMAGE_LO_RES_0_VERTEX,                                     \
    BSGFX_IMAGE_LO_RES_1_COLOR,                                      \
    BSGFX_RENDERER_LO_RES

#define BSGFX_DEPENDENCY_SHADOWS                                     \
    BSGFX_DEPENDENCY_LO_RES,                                         \
    BSGFX_BUFFER_INSTANCE_METADATA,                                  \
    BSGFX_BATCH_VOLUME_COMPUTED,                                     \
    BSGFX_BATCH_PREFAB_SHADOWS,                                      \
    BSGFX_BATCH_VOLUME_DOWN_COMPUTED,                                \
    BSGFX_BATCH_VOLUME_INSTANCED,                                    \
    BSGFX_BATCH_VOLUME_SCREEN,                                       \
    BSGFX_QUEUE_COMPUTE

#define BSGFX_DEPENDENCY_HI_RES                                      \
    BSGFX_RENDERER_HI_RES,                                           \
    BSGFX_IMAGE_HI_RES_0_DEPTH

#define BSGFX_DEPENDENCY_FOLIAGE                                     \
    BSGFX_BATCH_FOLIAGE

#define BSGFX_DEPENDENCY_ATLAS                                       \
    BSGFX_SAMPLER_NEAREST,                                           \
    BSGFX_IMAGE_ANY

#define BSGFX_DEPENDENCY_INGAME                                      \
    BSGFX_DEPENDENCY_HI_RES,                                         \
    BSGFX_DEPENDENCY_LO_RES,                                         \
    BSGFX_DEPENDENCY_SHADOWS,                                        \
    BSGFX_DEPENDENCY_ATLAS,                                          \
    BSGFX_DEPENDENCY_FOLIAGE,                                        \
    BSGFX_DEPENDENCY_TILE,                                           \
    BSGFX_DEPENDENCY_PLAYER,                                         \
    BSGFX_RAY_TRACER_MAIN,                                           \
    BSGFX_IMAGE_RAYTRACE_TEST,                                       \
    BSGFX_BATCH_ITEMS,                                               \
    BSGFX_BUFFER_MATERIALS,                                          \
    BSGFX_BUFFER_MINIMAP,                                            \
    BSGFX_ATLAS_ANY,                                                 \
    BSGFX_BATCH_MESH_TYPE_VOLUME_COMPUTED,                           \
    /* images*/                                                      \
    BSGFX_IMAGE_64,                                                  \
    BSGFX_IMAGE_256,                                                 \
    BSGFX_IMAGE_512,                                                 \
    BSGFX_IMAGE_MINIMAP,                                             \
    /* batches */                                                    \
    BSGFX_BATCH_QUAD_INSTANCED,                                      \
    BSGFX_BATCH_SCREEN,                                              \
    BSGFX_BATCH_SPHERE_INSTANCED,                                    \
    BSGFX_BATCH_PRIMITIVE_TILES,                                     \
    /* fonts */                                                      \
    BSGFX_FONT_STANDARD,                                             \
    BSGFX_FONT_TEMPLE,                                               \
    BSGFX_FONT_CONSOLAS,                                             \
    BSGFX_FONT_ARIAL_16,

#define BSGFX_DEPENDENCY_TEXT                                        \
    BSGFX_BATCH_QUAD_INSTANCED,                                      \
    BSGFX_BATCH_TEXT_INSTANCED

#define BSGFX_DEPENDENCY_DEBUG                                       \
    BSGFX_DEPENDENCY_TEXT,                                           \
    BSGFX_BUFFER_LO_RES_CURSOR_READS,                                \
    BSGFX_BUFFER_LO_RES_SCREENSHOT,                                  \
    BSGFX_FONT_SMALL,                                                \
    BSGFX_BATCH_LINE_INSTANCED,                                      \
    BSGFX_BATCH_POINT_INSTANCED

#define BSGFX_REQUIRED_FOR_SHADOW_VOLUMES                            \
    .stencil_front = {                                               \
        .fail_op = BS_STENCIL_OP_KEEP,                               \
        .depth_fail_op = BS_STENCIL_OP_KEEP,                         \
        .pass_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,                \
        .compare_mask = 0xFF,                                        \
        .compare_op = BS_COMPARE_OP_LESS_OR_EQUAL,                   \
        .write_mask = 0xFF,                                          \
    },                                                               \
    .stencil_back = {                                                \
        .fail_op = BS_STENCIL_OP_KEEP,                               \
        .depth_fail_op = BS_STENCIL_OP_KEEP,                         \
        .pass_op = BS_STENCIL_OP_INCREMENT_AND_CLAMP,                \
        .compare_mask = 0xFF,                                        \
        .compare_op = BS_COMPARE_OP_LESS_OR_EQUAL,                   \
        .write_mask = 0xFF,                                          \
    },                                                               \
    .cull_type = BS_CULL_MODE_NONE

#define BSGFX_REQUIRED_FOR_SHADOW_STENCIL                            \
    .depth_comparison = BS_COMPARE_OP_ALWAYS,                        \
    .stencil_front = {                                               \
        .compare_op = BS_COMPARE_OP_EQUAL,                           \
        .compare_mask = 0xFF,                                        \
        .fail_op = BS_STENCIL_OP_KEEP,                               \
        .depth_fail_op = BS_STENCIL_OP_KEEP,                         \
        .pass_op = BS_STENCIL_OP_KEEP,                               \
    }

#define BSGFX_TRANSPARENT_OPTIONS                                    \
    .src_alpha_factor = BS_BLEND_FACTOR_SRC_ALPHA,                   \
    .dst_alpha_factor = BS_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,         \
    .src_color_factor = BS_BLEND_FACTOR_SRC_ALPHA,                   \
    .dst_color_factor = BS_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,         \
    .color_op = BS_BLEND_OP_ADD,                                     \
    .alpha_op = BS_BLEND_OP_ADD

#define BSGFX_COLLIDER_SKIN                                          \
    (0.015)

#define du_animatorCallback(name)                                    \
    void(*##name)(bsgfx_Animator*)

#define BSGFX_ALPHABET_DEFAULT                                       \
    " !\"#$%&'()*+,-./0123456789:;<=>?@ABCDEFGHIJKLMNOPQRSTUVWXYZ[\\]^_`abcdefghijklmnopqrstuvwxyz{|}~"

#define BSGFX_CONTENT_PATH                                           \
    "content/bsgfx"

#define BSGFX_PARSE_FORMAT(format, data, len)                        \
        do {                                                         \
            va_list args;                                            \
            va_start(args, format);                                  \
            len = vsnprintf(NULL, 0, format, args);                  \
            va_end(args);                                            \
            if (len >= (sizeof(data) / sizeof(*data))) {             \
                bs_throwBasiliskF(BSX_OUT_OF_BOUNDS,                 \
                    "Format %s has a limit of %d characters",        \
                    format, sizeof(data) - 1);                       \
            }                                                        \
            va_start(args, format);                                  \
            vsprintf(data, format, args);                            \
            va_end(args);                                            \
        } while (0)

#define BSGFX_MAX_NUMBER_PLAYERS                                     \
    (8)

#define BSGFX_PIXEL_SCALE                                            \
    (4.0)

#define BSGFX_FIXED_TICK                                             \
    (1.0 / 40.0)

#define BSGFX_TITLE_BAR_COLOR                                        \
    (bs_RGBA) { 255, 255, 255, 200 }

#define BSGFX_SCREENSHOT_DIMENSIONS                                  \
    bs_iv2(1920 / 4, 1080 / 4)

#define BSGFX_MOTD                                                   \
    "poser\n"                                                        \
    "pre-pre-alpha\n\n"                                              \
    "thx for playing\n"                                              \
    "special thx malteSWE!"

#define BSGFX_SHADOW_COLOR                                           \
    ((bs_RGBA) { 50, 50, 60, 150 })

#define BSGFX_RGBA(r, g, b, a)                                       \
    ((bs_vec4) { (float)r / 255.0, (float)g / 255.0, (float)b / 255.0, (float)a / 255.0 })

#define BSGFX_WHITE                                                  \
    BSGFX_RGBA(255, 255, 255, 255)

#define BSGFX_TILE_SIZE                                              \
    ((bs_vec2) { 32, 32 })

#define BSGFX_TEXTURE_PATH                                           \
    "resources/textures/"

#define BSGFX_MODEL_SUBTYPE(X)                                       \
    X(BSGFX_MODEL_SUBTYPE_DEFAULT)                                   \
    X(BSGFX_MODEL_SUBTYPE_MIRRORED)                                  \
    X(BSGFX_MODEL_SUBTYPE_COUNT)

#define BSGFX_FOREACH_PROC(X)                                        \
    X(PFN_void, bsmod_onIni)                                         \
    X(PFN_void, bsmod_onLateIni)                                     \
    X(PFN_void, bsmod_onLoad)                                        \
    X(PFN_void, bsmod_copyHoveringDataToBuffer)                      \
    X(PFN_bsmod_onCreateSubtypes, bsmod_onCreateQuadSubtypes)        \
    X(PFN_bsmod_onQueue, bsmod_onQueue)                              \
    X(PFN_void, bsmod_onGfxRender)                                   \
    X(PFN_bsmod_isSelected, bsmod_isSelected)                        \
    X(PFN_bsmod_onMap, bsmod_onMap)                                  \
    X(PFN_void, bsmod_onTrack)

#define BSGFX_STRUCT_GEN(TYPE, FUNC, ...)                            \
    TYPE FUNC;

#define BSGFX_COUNT_GEN(TYPE, FUNC, ...)                             \
    +1

typedef enum bsgfx_Id bsgfx_Id;
typedef bs_U32 bsgfx_ColliderFlags;
typedef void (__stdcall* PFN_void)();
typedef bs_Queue* (__stdcall* PFN_bsmod_onQueue)();
typedef bool (__stdcall* PFN_bsmod_isSelected)(enum bsgfx_TypeId type, int id);
typedef bool (__stdcall* PFN_bs_hoveringGrid)();
typedef void (__stdcall* PFN_bsmod_onMap)(enum bsgfx_TypeId type_id, int id);
typedef void (__stdcall* PFN_bsmod_onCreateSubtypes)(bs_Range range);
struct bsgfx_Scene {
    const char* name;
    bs_U64 name_hash;
};

struct bsgfx_Material {
    bs_U64 hash;
    char* name;
    bsgfx_MaterialCategory category;
    bool highlighted, auto_unhighlight;
    bool editable;
    int id;
    int json_index;
    bsgfx_MaterialContract* contract;
};

struct bsgfx_Collider {
    bsgfx_ColliderFlags collision;
    bs_U32 batch_offset;
    bs_vec3 velocity_old;
    bs_vec3 normal;
    float angle;
    float angle_old;
    float length;
    float sweep_radius;
    bs_ivec3 resolution;
    bs_vec3 scale;
    bs_Aabb aabb;
    bool active;
};

struct bsgfx_SweepCollision {
    bs_vec3 position;
    bs_vec4 rotation;
    bs_vec3 scale;
    bs_U32 flags;
};

struct bsgfx_Animator {
    float time;
    float previous_time;
    float blend_factor;
    int queued_animation_id;
    int current_animation_id;
    int current_blend_animation_id;
    bs_Armature* armature;
    bs_U32 skeleton;
    bs_Animation* animations;
    int animations_count;
    void (*exit)();
};

struct bsgfx_AnimatorCallbacks {
    du_animatorCallback(once) ;
    du_animatorCallback(once_per_tick) ;
    du_animatorCallback(once_per_loop) ;
    du_animatorCallback(once_per_exit) ;
    du_animatorCallback(pose) ;
};

struct bsgfx_InstanceBuffer {
    struct bsgfx_InstanceHeader header;
    char data[64];
};

struct bsgfx_MeshInstance {
    bs_mat4 transform;
};

struct bsgfx_BoneInstance {
    bs_mat4 transform;
};

struct bsgfx_QuadInstance {
    bs_mat4x3 transform;
    bs_vec2 coords;
    bs_vec2 offset;
};

struct bsgfx_Text {
    bs_vec4 position;
    float scale;
    float max_length;
    bs_U32 flags;
    int select_start, select_end;
    int material_id;
    bs_RGBA colors[8];
};

struct bsgfx_Settings {
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
};

struct Poser {
    int day;
    int hour;
    int minute;
    int second;
    float time;
    bs_vec3 sun_direction;
    struct {
        bs_mat4 proj;
        bs_mat4 view;
        bs_mat4 result;
    } camera;
    struct {
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
    struct {
        int joint_offset;
    } members[BSGFX_MAX_NUMBER_PLAYERS];
    int member_count;
    bs_List sweep_collisions;
    bool menu_blocked;
    bool menu_open;
};

struct bsgfx_Procedures {
    BSGFX_FOREACH_PROC(BSGFX_STRUCT_GEN) ;
};

struct bsgfx_Callbacks {
    PFN_void tick;
    PFN_void fixedTick;
    PFN_void render;
    PFN_void ini;
    PFN_void loadScene;
};

struct bsgfx_Application {
    const char* name;
};

enum bsgfx_Subtype {
    BSGFX_SUBTYPE_BIPYRAMID,
    BSGFX_SUBTYPE_PRIMITIVE_BOX,
    BSGFX_SUBTYPE_PRIMITIVE_SPHERE,
    BSGFX_SUBTYPE_TILE,
    BSGFX_SUBTYPE_TILE_2,
    BSGFX_SUBTYPE_64_HI,
    BSGFX_SUBTYPE_256_HI,
    BSGFX_SUBTYPE_ATLAS_HI,
    BSGFX_SUBTYPE_UI,
    BSGFX_SUBTYPE_UI_COLOR,
    BSGFX_SUBTYPE_UI_STENCIL,
    BSGFX_SUBTYPE_COLOR_PICKER,
    BSGFX_SUBTYPE_COLOR_PICKER_HUE,
    BSGFX_SUBTYPE_COLOR_PICKER_ALPHA,
    BSGFX_SUBTYPE_DITHER,
    BSGFX_SUBTYPE_CORNER_GRADIENT,
    BSGFX_SUBTYPE_TILE_HI,
    BSGFX_SUBTYPE_QUAD_MATERIAL_TEXTURE,
    BSGFX_SUBTYPE_PRIMITIVE_HI,
    BSGFX_SUBTYPE_ATLAS_PREFAB,
    BSGFX_SUBTYPE_ATLAS_PREFAB_TRANSPARENT,
    BSGFX_SUBTYPE_PLANE_MESH,
    BSGFX_SUBTYPE_ATLAS,
    BSGFX_SUBTYPE_CONE_MESH,
    BSGFX_SUBTYPE_SPHERE_MESH,
    BSGFX_SUBTYPE_POINT,
    BSGFX_SUBTYPE_LINE,
    BSGFX_SUBTYPE_LINE_2D,
    BSGFX_SUBTYPE_LINE_DEPTHLESS,
    BSGFX_SUBTYPE_COUNT,
};

enum bsgfx_ResourceType {
    BSGFX_RESOURCE_PRIMITIVE = BS_RESOURCE_TYPE_COUNT,
    BSGFX_RESOURCE_PREFAB,
    BSGFX_RESOURCE_TILE,
    BSGFX_RESOURCE_LIGHT,
    BSGFX_RESOURCE_TYPE_COUNT,
};

enum bsgfx_ImageId {
    BSGFX_IMAGE_LO_RES_0_DEPTH,
    BSGFX_IMAGE_LO_RES_0_COLOR,
    BSGFX_IMAGE_LO_RES_0_NORMAL,
    BSGFX_IMAGE_LO_RES_0_POSITION,
    BSGFX_IMAGE_LO_RES_0_INDEX,
    BSGFX_IMAGE_LO_RES_0_FLAGS,
    BSGFX_IMAGE_LO_RES_0_VERTEX,
    BSGFX_IMAGE_LO_RES_1_COLOR,
    BSGFX_IMAGE_HI_RES_0_DEPTH,
    BSGFX_IMAGE_TILE,
    BSGFX_IMAGE_ANY,
    BSGFX_IMAGE_64,
    BSGFX_IMAGE_256,
    BSGFX_IMAGE_512,
    BSGFX_IMAGE_RAYTRACE_TEST,
    BSGFX_IMAGE_LEVEL_THUMBNAIL,
    BSGFX_IMAGE_MINIMAP,
    BSGFX_IMAGES_COUNT,
};

enum bsgfx_SamplerId {
    BSGFX_SAMPLER_NEAREST,
    BSGFX_SAMPLER_LINEAR,
    BSGFX_SAMPLERS_COUNT,
};

enum bsgfx_BufferId {
    BSGFX_BUFFER_JOINTS,
    BSGFX_BUFFER_INSTANCE_METADATA,
    BSGFX_BUFFER_LO_RES_CURSOR_READS,
    BSGFX_BUFFER_LO_RES_SCREENSHOT,
    BSGFX_BUFFER_MATERIALS,
    BSGFX_BUFFER_MINIMAP,
    BSGFX_BUFFER_BINDING_TABLE,
    BSGFX_BUFFERS_COUNT,
};

enum bsgfx_BatchId {
    BSGFX_BATCH_VOLUME_COMPUTED,
    BSGFX_BATCH_PREFAB_SHADOWS,
    BSGFX_BATCH_VOLUME_INSTANCED,
    BSGFX_BATCH_VOLUME_SCREEN,
    BSGFX_BATCH_SCREEN,
    BSGFX_BATCH_LINE_INSTANCED,
    BSGFX_BATCH_POINT_INSTANCED,
    BSGFX_BATCH_SPHERE_INSTANCED,
    BSGFX_BATCH_BONE_INSTANCED,
    BSGFX_BATCH_MESH_INSTANCED,
    BSGFX_BATCH_MESH_TYPE_VOLUME_COMPUTED,
    BSGFX_BATCH_FOLIAGE,
    BSGFX_BATCH_QUAD_INSTANCED,
    BSGFX_BATCH_TEXT_INSTANCED,
    BSGFX_BATCH_ITEMS,
    BSGFX_BATCH_PRIMITIVE_TILES,
    BSGFX_BATCHES_COUNT,
};

enum bsgfx_RendererId {
    BSGFX_RENDERER_LO_RES,
    BSGFX_RENDERER_HI_RES,
    BSGFX_RENDERERS_COUNT,
};

enum bsgfx_QueueId {
    BSGFX_QUEUE_GRAPHICS,
    BSGFX_QUEUE_COMPUTE,
    BSGFX_QUEUES_COUNT,
};

enum bsgfx_RayTracerId {
    BSGFX_RAY_TRACER_MAIN,
    BSGFX_RAY_TRACERS_COUNT,
};

enum bsgfx_AtlasId {
    BSGFX_ATLAS_ANY,
    BSGFX_ATLASES_COUNT,
};

enum bsgfx_FontId {
    BSGFX_FONT_STANDARD,
    BSGFX_FONT_SMALL,
    BSGFX_FONT_TEMPLE,
    BSGFX_FONT_CONSOLAS,
    BSGFX_FONT_ARIAL_16,
    BSGFX_FONT_ARIAL_24,
    BSGFX_FONT_ARIAL_32,
    BSGFX_FONT_ARIAL_48,
    BSGFX_FONTS_COUNT,
};

enum bsgfx_MaterialCategory {
    BSGFX_MATERIAL_CATEGORY_NONE,
    BSGFX_MATERIAL_CATEGORY_UI_COLOR_SCHEME,
    BSGFX_MATERIAL_CATEGORY_COUNT,
};

enum bsgfx_CollisionType {
    BSGFX_COLLISION_NONE = 0,
    BSGFX_COLLISION_SLOPE = 1 << 0,
    BSGFX_COLLISION_SLOPE_DESCENT = 1 << 1,
    BSGFX_COLLISION_ABOVE = 1 << 2,
    BSGFX_COLLISION_BELOW = 1 << 3,
    BSGFX_COLLISION_LEFT = 1 << 4,
    BSGFX_COLLISION_RIGHT = 1 << 5,
    BSGFX_COLLISION_BACK = 1 << 6,
    BSGFX_COLLISION_FRONT = 1 << 7,
    BSGFX_COLLISION_MAX_SLOPE = 1 << 8,
    BSGFX_COLLISION_STAIRS = 1 << 9,
};

enum bsgfx_AnimatorTypeBit {
    BSGFX_ANIMATOR_TYPE_ONCE_BIT = 1 << 0,
    BSGFX_ANIMATOR_TYPE_ONCE_PER_TICK_BIT = 1 << 1,
    BSGFX_ANIMATOR_TYPE_ONCE_PER_LOOP_BIT = 1 << 2,
    BSGFX_ANIMATOR_TYPE_ONCE_PER_EXIT_BIT = 1 << 3,
};

enum bsgfx_FontFlag {
    BSGFX_FONT_FORCE_CREATE = 1 << 0,
    BSGFX_FONT_NO_CAPS = 1 << 1,
};

enum bsgfx_ModelSubtype {
    BSGFX_MODEL_SUBTYPE_DEFAULT,
    BSGFX_MODEL_SUBTYPE_MIRRORED,
    BSGFX_MODEL_SUBTYPE_COUNT,
};

enum bsgfx_Pipe {
    BSGFX_GRAPHICS_PIPE = (1 << 0),
    BSGFX_COMPUTE_PIPE = (1 << 1),
    BSGFX_SHADOW_PIPE = (1 << 2),
    BSGFX_BONE_PIPE = (1 << 3),
    BSGFX_MESH_PIPE = (1 << 4),
    BSGFX_RENDER_LINES = (1 << 5),
    BSGFX_RENDER_TILES = (1 << 6),
    BSGFX_ALL_PIPES = 0xFFFFFFFF,
};

 /**
  @return bsgfx_Scene*
  */
BSGFXAPI bsgfx_Scene*
bsgfx_currentScene();

 /**
  @param name
  @return void
  */
BSGFXAPI void
bsgfx_loadScene(
    const char* name);

 /**
  @return int
  */
BSGFXAPI int
bsgfx_images();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_samplers();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_buffers();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_batches();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_renderers();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_queues();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_rayTracers();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_atlases();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_fonts();

 /**
  @param category
  @return const char*
  */
BSGFXAPI const char*
bsgfx_materialCategoryName(
    bsgfx_MaterialCategory category);

 /**
  @return bs_List*
  */
BSGFXAPI bs_List*
bsgfx_materials();

 /**
  @param hash
  @return bsgfx_Material*
  */
BSGFXAPI bsgfx_Material*
bsgfx_queryMaterialHash(
    bs_U64 hash);

 /**
  @param id
  @return char*
  */
BSGFXAPI char*
bsgfx_materialName(
    int id);

 /**
  @param name
  @return bsgfx_Material*
  */
BSGFXAPI bsgfx_Material*
bsgfx_queryMaterial(
    const char* name);

 /**
  @param id
  @return bs_U64
  */
BSGFXAPI bs_U64
bsgfx_materialHash(
    int id);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_allocateMaterials();

 /**
  @return void
  */
BSGFXAPI void
bsgfx_loadMaterials();

 /**
  @param name
  @param name_length
  @return bsgfx_Material*
  */
BSGFXAPI bsgfx_Material*
bsgfx_material(
    char* name,
    int name_length);

 /**
  @param format
  @param args
  @return bsgfx_Material*
  */
BSGFXAPI bsgfx_Material*
bsgfx_materialV(
    char* format,
    va_list args);

 /**
  @param format
  @param ...
  @return bsgfx_Material*
  */
BSGFXAPI bsgfx_Material*
bsgfx_materialF(
    char* format,
     ...);

 /**
  @param material_id
  @param auto_unhighlight
  @return void
  */
BSGFXAPI void
bsgfx_highlightMaterial(
    int material_id,
    bool auto_unhighlight);

 /**
  @param material_id
  @return void
  */
BSGFXAPI void
bsgfx_unhighlightMaterial(
    int material_id);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_tickMaterials();

 /**
  @return void
  */
BSGFXAPI void
bsgfx_shadowPipe();

 /**
  @return void
  */
BSGFXAPI void
bsgfx_pipeline();

 /**
  @return void
  */
BSGFXAPI void
bsgfx_onDeviceLost();

 /**
  @param sweep_radius
  @param position
  @return void
  */
BSGFXAPI void
bsgfx_sweepCollisions(
    float sweep_radius,
    bs_vec3 position);

 /**
  @param aabb
  @param scale
  @param resolution
  @param sweep_radius
  @return bsgfx_Collider
  */
BSGFXAPI bsgfx_Collider
bsgfx_collider(
    bs_Aabb aabb,
    bs_vec3 scale,
    bs_ivec3 resolution,
    float sweep_radius);

 /**
  @param collider
  @param position
  @param velocity
  @return void
  */
BSGFXAPI void
bsgfx_applyCollisions(
    bsgfx_Collider* collider,
    bs_vec3 position,
    bs_vec3* velocity);

 /**
  @param collider
  @return void
  */
BSGFXAPI void
bsgfx_printCollisions(
    bsgfx_Collider* collider);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_instanceSweepCollisions();

 /**
  @param collider
  @param position
  @param velocity
  @return void
  */
BSGFXAPI void
bsgfx_instanceCollider(
    bsgfx_Collider* collider,
    bs_vec3 position,
    bs_vec3* velocity);

 /**
  @param armature
  @return int
  */
BSGFXAPI int
bsgfx_skeleton(
    bs_Armature* armature);

 /**
  @param animation
  @param time
  @param num_frames
  @return int
  */
BSGFXAPI int
bsgfx_animationFrame(
    bs_Animation* animation,
    float time,
    int num_frames);

 /**
  @param animation
  @param time_scale
  @return int
  */
BSGFXAPI int
bsgfx_animationFrameCount(
    bs_Animation* animation,
    float time_scale);

 /**
  @param animator
  @param velocity
  @param input
  @return void
  */
BSGFXAPI void
bsgfx_applyAnimationVelocity(
    bsgfx_Animator* animator,
    bs_vec3* velocity,
    bs_vec2 input);

 /**
  @param animator
  @param animation_id
  @return void
  */
BSGFXAPI void
bsgfx_queueAnimation(
    bsgfx_Animator* animator,
    int animation_id);

 /**
  @param animator
  @param callbacks
  @return void
  */
BSGFXAPI void
bsgfx_runAnimator(
    bsgfx_Animator* animator,
    bsgfx_AnimatorCallbacks callbacks);

 /**
  @param armature
  @param resting_animation_id
  @param animations_count
  @return bsgfx_Animator
  */
BSGFXAPI bsgfx_Animator
bsgfx_animator(
    bs_Armature* armature,
    int resting_animation_id,
    int animations_count);

 /**
  @param subtype
  @return bs_List*
  */
BSGFXAPI bs_List*
bsgfx_subtypeInstances(
    int subtype);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_iniInstances();

 /**
  @param type
  @param max_instance_count
  @param bind_set
  @param binding
  @return void
  */
BSGFXAPI void
bsgfx_instanceType(
    int type,
    int max_instance_count,
    int bind_set,
    int binding);

 /**
  @param subtype
  @return bs_Range
  */
BSGFXAPI bs_Range
bsgfx_subtypeRange(
    int subtype);

 /**
  @param subtype
  @return void
  */
BSGFXAPI void
bsgfx_deleteSubtype(
    int subtype);

 /**
  @param subtype
  @return int
  */
BSGFXAPI int
bsgfx_instanceCount(
    int subtype);

 /**
  @param instance_type_id
  @return int
  */
BSGFXAPI int
bsgfx_subtypeCount(
    int instance_type_id);

 /**
  @return const int*
  */
BSGFXAPI const int*
bsgfx_subtypes();

 /**
  @param type
  @param batch
  @param flags
  @param range
  @return int
  */
BSGFXAPI int
bsgfx_subtype(
    int type,
    bs_Batch* batch,
    bs_U32 flags,
    bs_Range range);

 /**
  @param subtype
  @param data
  @param data_size
  @param flags
  @param bone_index
  @param id
  @param material
  @return int
  */
BSGFXAPI int
bsgfx_instance(
    int subtype,
    const char* data,
    int data_size,
    bs_U32 flags,
    unsigned int bone_index,
    int id,
    int material);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_tickInstances();

 /**
  @param subtype
  @param flag
  @return bool
  */
BSGFXAPI bool
bsgfx_subtypeHasFlag(
    int subtype,
    bs_U32 flag);

 /**
  @param subtype
  @param pipeline
  @return void
  */
BSGFXAPI void
bsgfx_renderSubtype(
    int subtype,
    bs_Pipeline* pipeline);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_resetInstances();

 /**
  @param subtype
  @return void
  */
BSGFXAPI void
bsgfx_resetSubtype(
    int subtype);

 /**
  @param mesh
  @param position
  @param rotation
  @param scale
  @param subtype_offset
  @param origin_at_center
  @return void
  */
BSGFXAPI void
bsgfx_instanceHiResMesh(
    bs_Mesh* mesh,
    bs_vec3 position,
    bs_vec4 rotation,
    float scale,
    int subtype_offset,
    bool origin_at_center);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_renderFontSubtypes();

 /**
  @param result
  @return void
  */
BSGFXAPI void
bsgfx_settingsEditor(
    bs_List* result);

 /**
  @return void
  */
BSGFXAPI void
bsgfx_renderFineShadowVolumes();

 /**
  @return void
  */
BSGFXAPI void
bsgfx_renderShadowVolumes();

 /**
  @return void
  */
BSGFXAPI void
bsgfx_computeShadowVolumes();

 /**
  @return bs_Model*
  */
BSGFXAPI bs_Model*
bsgfx_prefabModel();

 /**
  @return int
  */
BSGFXAPI int
bsgfx_package();

 /**
  @param name
  @param width
  @param height
  @param argc
  @param argv
  @return void
  */
BSGFXAPI void
bsgfx_ini(
    const char* name,
    bs_U32 width,
    bs_U32 height,
    int argc,
    char* argv[]);

 /**
  @return HINSTANCE
  */
BSGFXAPI HINSTANCE
bsgfx_bsmodDll();

 /**
  @param result
  @return void
  */
BSGFXAPI void
bsgfx_checkGFSDK(
    bs_U32 result);

 /**
  @param result
  @return void
  */
BSGFXAPI void
bsgfx_logGFSDK(
    bs_U32 result);

 /**
  @return bsgfx_Application*
  */
BSGFXAPI bsgfx_Application*
bsgfx_app();

 /**
  @return bsgfx_Callbacks*
  */
BSGFXAPI bsgfx_Callbacks*
bsgfx_callbacks();

 /**
  @return bsgfx_Settings*
  */
BSGFXAPI bsgfx_Settings*
bsgfx_settings();

 /**
  @return struct Poser*
  */
BSGFXAPI struct Poser*
poser();

 /**
  @param proj
  @param view
  @return void
  */
BSGFXAPI void
bsgfx_setCamera(
    const bs_mat4* proj,
    const bs_mat4* view);

BSGFXAPI extern bsgfx_Scene _bsgfx_current_scene_;
BSGFXAPI extern int _bsgfx_images_;
BSGFXAPI extern int _bsgfx_samplers_;
BSGFXAPI extern int _bsgfx_buffers_;
BSGFXAPI extern int _bsgfx_batches_;
BSGFXAPI extern int _bsgfx_renderers_;
BSGFXAPI extern int _bsgfx_queues_;
BSGFXAPI extern int _bsgfx_ray_tracers_;
BSGFXAPI extern int _bsgfx_atlases_;
BSGFXAPI extern int _bsgfx_fonts_;
BSGFXAPI extern int _bsgfx_subtypes_[BSGFX_SUBTYPE_COUNT];
BSGFXAPI extern int _bsgfx_material_categories_[BSGFX_MATERIAL_CATEGORY_COUNT];
BSGFXAPI extern bs_List _bsgfx_materials_;
BSGFXAPI extern bs_mat4* _bsgfx_shader_joints_;
BSGFXAPI extern bs_U32 _bsgfx_num_shader_joints_;
BSGFXAPI extern bsgfx_Settings _bsgfx_settings_;
BSGFXAPI extern struct Poser* _poser_;
BSGFXAPI extern bsgfx_Procedures _bsgfx_procs_;
BSGFXAPI extern bsgfx_Application _bsgfx_app_;
BSGFXAPI extern bs_Model* _bsgfx_prefab_model_;
BSGFXAPI extern bsgfx_Callbacks _bsgfx_callbacks_;
BSGFXAPI extern HINSTANCE _bsgfx_bsmod_dll_;
BSGFXAPI extern bs_String* _bsgfx_variadic_;
BSGFXAPI extern int _bsgfx_package_;

#endif
