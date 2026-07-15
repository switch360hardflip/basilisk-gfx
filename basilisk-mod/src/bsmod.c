#include <stdio.h>
#include <bsmod.h>
#include <bsmod_lisk.h>
#include <bsmod_type.h>
#include <bsmod_track.h>
#include <bsmod_compile.h>
#include <ui/bsgfx_ui.h>
#include <bsmod_bpak.h>
#include <bsgfx.h>
#include <bsgfx_cache.h>
#include <bsmod_transform.h>
#include <ui/bsmod_ui.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/prefab/bsgfx_prefab.h>
#include <types/tile/bsgfx_tile.h>

struct Bsmod bsmod = {
    .bsgfx_package = -1,
    .package = -1,
    .history = BS_I64_MAX,
    .hovering = {
        .prefab = -1,
        .primitive = -1,
    },
    .selected_type = -1,
    .axis = -1,
    .dragging_id = -1,
    .dragging_subtype = -1,
    .selected_ids = {.unit_size = sizeof(int), .increment = 32 },
    .selected_tiles = {.unit_size = sizeof(int), .increment = 32 },
};

bs_Json _bsmod_config = { 0 };

int _bsmod_images = -1, _bsmod_samplers = -1, _bsmod_buffers = -1,
    _bsmod_batches = -1, _bsmod_renderers = -1, _bsmod_ray_tracers = -1,
    _bsmod_queues = -1, _bsmod_atlases = -1, _bsmod_fonts = -1;

volatile long _bsmod_has_performed_tracked_changes = 1;

bsmod_Procedures _bsmod_procs = { 0 };
int _bsmod_subtypes[BSMOD_SUBTYPE_COUNT] = { 0 };

bool bsmod_keyHeld(bs_U32 code) {
    return code > BS_KEYS_COUNT ? false : BS_GET_BIT(bs_io()->hold_keys, code);
}

bool bsmod_keyDown(bs_U32 code) {
    return code > BS_KEYS_COUNT ? false : BS_GET_BIT(bs_io()->keys, code);
}

bool bsmod_keyDownOnce(bs_U32 code) {
    return code > BS_KEYS_COUNT ? false : (BS_GET_BIT(bs_io()->keys, code) && !BS_GET_BIT(bs_io()->keys_old, code));
}

bool bsmod_keyUpOnce(bs_U32 code) {
    return code > BS_KEYS_COUNT ? false : (!BS_GET_BIT(bs_io()->keys, code) && BS_GET_BIT(bs_io()->keys_old, code));
}

bool bsmod_charDown(unsigned char code) {
    return BS_GET_BIT(bs_io()->chars, code);
}

bool bsmod_charDownOnce(unsigned char code) {
    return code > BS_KEY_BYTES_COUNT ? false : (BS_GET_BIT(bs_io()->chars, code) && !BS_GET_BIT(bs_io()->chars_old, code));
}

bool bsmod_charUpOnce(unsigned char code) {
    return code > BS_KEY_BYTES_COUNT ? false : (!BS_GET_BIT(bs_io()->chars, code) && BS_GET_BIT(bs_io()->chars_old, code));
}

bool bsmod_middleClick() { return bs_io()->middle_clicked; }
bool bsmod_middleClickOnce() { return bs_io()->middle_clicked && !bs_io()->middle_clicked_last; }
bool bsmod_middleClickUpOnce() { return !bs_io()->middle_clicked && bs_io()->middle_clicked_last; }
bool bsmod_leftClick() { return bs_io()->left_clicked; }
bool bsmod_rightClick() { return bs_io()->right_clicked; }
bool bsmod_rightClickOnce() { return bs_io()->right_clicked && !bs_io()->right_clicked_last; }
bool bsmod_leftClickOnce() { return bs_io()->left_clicked && !bs_io()->left_clicked_last; }
bool bsmod_rightClickUpOnce() { return !bs_io()->right_clicked && bs_io()->right_clicked_last; }
bool bsmod_leftClickUpOnce() { return !bs_io()->left_clicked && bs_io()->left_clicked_last; }

static void bsmod_instanceAxisFace(
    bsgfx_Primitive* primitive,
    int axis,
    bs_RGBA color,
    bs_mat4* transform)
{
    const bsgfx_TileAxis* ax = bsgfx_tileAxes() + axis;

    int width = (int)(primitive->scale.a[ax->width_axis] * 2.0f);
    int height = (int)(primitive->scale.a[ax->height_axis] * 2.0f);

    bs_vec3 right = bs_qRotateVec3(&primitive->rotation, &ax->right);
    bs_vec3 up = bs_qRotateVec3(&primitive->rotation, &ax->up);

    bs_vec3 m = bs_v3Mul(ax->start_sign, primitive->scale);
    bs_vec3 start = bs_qRotateVec3(&primitive->rotation, &m);

    start = bs_v3Add(start, primitive->position);

    bs_vec3 full_right = bs_v3MulS(right, (float)width);
    bs_vec3 full_up = bs_v3MulS(up, (float)height);

    bs_vec3 a = start;
    bs_vec3 b = bs_v3Add(a, full_right);
    bs_vec3 c = bs_v3Add(a, full_up);
    bs_vec3 d = bs_v3Add(c, full_right);

    bsgfx_transformedDepthlessLineInstance(a, b, color, transform);
    bsgfx_transformedDepthlessLineInstance(b, d, color, transform);
    bsgfx_transformedDepthlessLineInstance(d, c, color, transform);
    bsgfx_transformedDepthlessLineInstance(c, a, color, transform);
}

void bsmod_onTick() {
    memset(&bsmod.queue, 0, sizeof(bsmod.queue));

  //  static float lisk_time = 0.0;
  //  lisk_time += bs_elapsedTime();
  //  if (lisk_time > 0.1) {
  //      if (bs_arguments()->use_lisk)
  //          bsmod_tickLisk();
  //      lisk_time = 0.0;
  //  }

    if (InterlockedCompareExchange(&_bsmod_has_performed_tracked_changes, 1, 1) == 1) {

        bsmod_savePackage(BSMOD_CONTENT_PATH);
        bsmod_savePackage(BSGFX_CONTENT_PATH);
        if (_bsmod_config.doc)
            bsmod_savePackage(bsmod_applicationContentPath());

    }

    bsmod_readHoveringOutputs();
    bsmod_readHoveringInstanceData();
    bsmod_readHoveringVertex();
    bsmod_instanceUI();
    if (!bsmod.ui_blocked)
        bsmod_instanceTransform();
    if (!bsmod.ui_blocked)
        bsmod_selectHoveringTypes();
    if (!bsmod.ui_blocked)
        bsmod_editSelectedType();
    if (bsmod_keyDown(BS_KEY_LEFT_CONTROL) && bsmod_keyDownOnce(BS_KEY_C))
        bsmod_deselectAll();

    static int last_selected_count = 0;
    static int last_selected_type = 0;
    switch (bsmod.selected_type) {
    case BSGFX_TYPE_TILE: {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, bsmod.selected_tile_primitive);

        if (last_selected_type != bsmod.selected_type || last_selected_count != bsmod.selected_tiles.count) {
            bs_Batch* batch = bs_fetch(_bsmod_batches, BSMOD_BATCH_TILE)->batch;
            bs_Scope last = *bs_getScope();
            bs_setScope(&(bs_Scope) { .queue = bs_singleTimesQueue() });
            bs_unpushBatch(batch);

            for (int i = 0; i < bsmod.selected_tiles.count; i++) {
                int* this = bs_fetchUnit(&bsmod.selected_tiles, i);
                bs_ivec2 coord = bsgfx_tileCoordinate(primitive, bsmod.selected_tile_axis, *this);
                if (coord.x == BS_I32_MAX)
                    continue;
                bsgfx_pushTileAt(batch, primitive, bsmod.selected_tile_axis, coord.x, coord.y, 0, 0);
            }

            bs_pushBatch(batch, BS_U32_MAX, BS_U32_MAX);
            bs_setScope(&last);



            /**

             */
            for (int i = 0; i < bsgfx_count(BSGFX_TYPE_TILE); i++) {
                bsgfx_Tile* tile = bsgfx_get(BSGFX_TYPE_TILE, i);

                for (int j = 0; j < bsmod.selected_tiles.count; j++) {
                    int id = *(int*)bs_fetchUnit(&bsmod.selected_tiles, j);
                    if (tile->index == id) {
                        bsmod_select(&bsmod.selected_ids, BSGFX_TYPE_TILE, i);
                    }
                }
            }

        }

        bs_mat4 transform = bs_transform(primitive->position, primitive->rotation, primitive->scale);
        bs_Aabb aabb = { .min = bs_v3V1(-1), .max = bs_v3V1(1) };
        bsgfx_obbInstance(&aabb, BS_WHITE, &transform);

        bsmod_instanceAxisFace(primitive, bsmod.selected_tile_axis, BS_RGBA(130, 245, 245, 255), &BS_MAT4_IDENTITY);

        last_selected_count = bsmod.selected_tiles.count;
        last_selected_type = bsmod.selected_type;
    } return;
    case BSGFX_TYPE_PRIMITIVE: {
        for (int i = 0; i < bsmod.selected_ids.count; i++) {
            int* this = bs_fetchUnit(&bsmod.selected_ids, i);
            bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, *this);

            bs_mat4 transform = bs_transform(primitive->position, primitive->rotation, primitive->scale);
            bs_Aabb aabb = { .min = bs_v3V1(-1), .max = bs_v3V1(1) };
            bsgfx_obbInstance(&aabb, BS_WHITE, &transform);
        }
    } break;
    case BSGFX_TYPE_PREFAB: {
        for (int i = 0; i < bsmod.selected_ids.count; i++) {
            int* this = bs_fetchUnit(&bsmod.selected_ids, i);
            bsgfx_Prefab* prefab = bsgfx_get(BSGFX_TYPE_PREFAB, *this);

            bs_mat4 transform = bs_transform(prefab->position, prefab->rotation, prefab->scale);
            bs_Aabb aabb = { .min = bs_v3V1(-1), .max = bs_v3V1(1) };

            bsgfx_instancePrefab(*this, BSGFX_PREFAB_SUBTYPE_MESH_POLYGON_OUTLINE);
        }
    } break;
    }

    end:
    last_selected_count = bsmod.selected_ids.count;
    last_selected_type = bsmod.selected_type;
}

void bsmod_onMap(bsgfx_TypeId type_id, int id) {
    switch (type_id) {
    case BSGFX_TYPE_PRIMITIVE:
        bsgfx_Primitive* primitive = bsgfx_get(type_id, id);
        if (bsmod.settings.draw_hidden_primitives && primitive->flags & BSGFX_PRIMITIVE_HIDDEN)
            primitive->flags &= ~BSGFX_PRIMITIVE_HIDDEN;
        break;
    }
}

static void bsmod_computeFlyCamera() {
    bs_vec2 resolution = {
        .x = (float)bs_resolution().x,
        .y = (float)bs_resolution().y
    };

    float move_speed = 0.15;
    const float mouse_sensitivity = 0.005;

    static float yaw = -3.8;
    static float pitch = -0.2;

    static bool rotating = false;
    static bs_vec2 last_cursor;

    bs_vec2 cursor = bs_cursorPosition();

    if (bsmod_keyDown(BS_KEY_LEFT_SHIFT))
        move_speed = 0.5;

    if (bsmod_middleClickOnce()) {
        rotating = true;
        last_cursor = cursor;
    }

    if (bsmod_middleClick()) {
        bs_vec2 delta = {
            last_cursor.x - cursor.x,
            last_cursor.y - cursor.y
        };

        yaw += delta.x * mouse_sensitivity;
        pitch -= delta.y * mouse_sensitivity;

        if (pitch > 1.5) pitch = 1.5;
        if (pitch < -1.5) pitch = -1.5;

        last_cursor = cursor;
    }

    if (bsmod_middleClickUpOnce()) {
        rotating = false;
    }

    bs_vec3 forward = {
        cosf(pitch) * sinf(yaw),
        sinf(pitch),
        cosf(pitch) * cosf(yaw)
    };

    static bs_vec3 camera_direction;
    static bs_vec3 camera_position = { 7.7, 12.7, 18.0 };

    camera_direction = bs_v3Normalize(forward);
    bs_vec3 right = bs_v3Normalize(bs_v3Cross(camera_direction, BS_V3(0.0, 1.0, 0.0)));
    bs_vec3 up = BS_V3(0.0, 1.0, 0.0);

    if (bsmod_keyDown(BS_KEY_W))
        camera_position = bs_v3Add(camera_position, bs_v3MulS(camera_direction, move_speed));

    if (bsmod_keyDown(BS_KEY_S))
        camera_position = bs_v3Sub(camera_position, bs_v3MulS(camera_direction, move_speed));

    if (bsmod_keyDown(BS_KEY_D))
        camera_position = bs_v3Add(camera_position, bs_v3MulS(right, move_speed));

    if (bsmod_keyDown(BS_KEY_A))
        camera_position = bs_v3Sub(camera_position, bs_v3MulS(right, move_speed));

    bs_mat4 proj = bs_perspective(
        bs_radians(50.0),
        resolution.x / resolution.y,
        0.1,
        1000.0
    );

    bs_mat4 view = bs_lookAt(
        camera_position,
        bs_v3Add(camera_position, camera_direction),
        BS_V3(0.0, 1.0, 0.0)
    );

    bsgfx_setCamera(&proj, &view);
}

void bsmod_onGfxRender() {
    static bool override_camera;

    if (BS_GET_BIT(bs_io()->keys, BS_KEY_TAB) && !BS_GET_BIT(bs_io()->keys_old, BS_KEY_TAB)) {
        bs_disableUserInputs(false);
        override_camera = !override_camera;

    }

    if (bsmod.ui_blocked)
        bs_lockCursorPosition(false);

    if (override_camera) {
        bs_lockCursorPosition(false);
        bsmod_computeFlyCamera();
        bs_disableUserInputs(true);
    }
}

static DWORD WINAPI bsmod_tickAsync(void* param) {
    while (1) {
        InterlockedExchange(&_bsmod_has_performed_tracked_changes, 0);
        bsmod_onTrack();
        InterlockedExchange(&_bsmod_has_performed_tracked_changes, 1);
        Sleep(1000);
    }
}

void bsmod_onLoadVariables() {
    if (_bsmod_config.doc)
        bs_destroyJson(&_bsmod_config);

    _bsmod_config = bs_loadJson(BSMOD_CONFIG_PATH);
    if (_bsmod_config.doc)
        bs_infoF("Loaded \"%s\"\n", BSMOD_CONFIG_PATH);

}

void bsmod_onIni() {
    _bsmod_images = bs_configureSource(BS_OBJECT_IMAGE, BSMOD_IMAGES_COUNT, (const char* []) { BSMOD_IMAGE_IDS(BS_STRING_GEN) });
    _bsmod_samplers = bs_configureSource(BS_OBJECT_SAMPLER, BSMOD_SAMPLERS_COUNT, (const char* []) { BSMOD_SAMPLER_IDS(BS_STRING_GEN) });
    _bsmod_buffers = bs_configureSource(BS_OBJECT_BUFFER, BSMOD_BUFFERS_COUNT, (const char* []) { BSMOD_BUFFER_IDS(BS_STRING_GEN) });
    _bsmod_queues = bs_configureSource(BS_OBJECT_QUEUE, BSMOD_QUEUES_COUNT, (const char* []) { BSMOD_QUEUE_IDS(BS_STRING_GEN) });
    _bsmod_batches = bs_configureSource(BS_OBJECT_BATCH, BSMOD_BATCHES_COUNT, (const char* []) { BSMOD_BATCH_IDS(BS_STRING_GEN) });
    _bsmod_renderers = bs_configureSource(BS_OBJECT_RENDERER, BSMOD_RENDERERS_COUNT, (const char* []) { BSMOD_RENDERER_IDS(BS_STRING_GEN) });
    _bsmod_ray_tracers = bs_configureSource(BS_OBJECT_RAY_TRACER, BSMOD_RAY_TRACERS_COUNT, (const char* []) { BSMOD_RAY_TRACER_IDS(BS_STRING_GEN) });
    _bsmod_fonts = bs_configureSource(BS_OBJECT_FONT, BSMOD_FONTS_COUNT, (const char* []) { BSMOD_FONT_IDS(BS_STRING_GEN) });
    _bsmod_atlases = bs_configureSource(BS_OBJECT_ATLAS, BSMOD_ATLASES_COUNT, (const char* []) { BSMOD_ATLAS_IDS(BS_STRING_GEN) });

    bsmod_onLoadVariables();
    bsmod_loadShaderReferences();

    bs_except(BSX_NOT_FOUND);
    bsmod.bsgfx_package = bs_loadPackage(BSGFX_CONTENT_PATH);
    if (!bs_except(0)) {
        bs_except(BSX_FAILED_TO_QUERY);
        bs_Resource* resource = bs_loadResource(bsmod.bsgfx_package, "bindings", 0);
        bs_except(0);

        if (resource) {
            bsmod.bindings_json = bs_json(resource->data->value, resource->data->len);
        }
    }

    if (!bsmod.bindings_json.doc)
        bsmod.bindings_json = bs_emptyJson();

    bsmod_iniPackage(BSMOD_CONTENT_PATH);
    bsmod_iniPackage(BSGFX_CONTENT_PATH);
    bsmod_iniPackage(bsmod_applicationContentPath());
    // bsmod_iniLisk();
    bsmod_iniCompiler();

    bsmod.bsgfx = LoadLibrary("basilisk-gfx.dll");
    if (bsmod.bsgfx) {
        bs_Procedure procedures[] = {
            BSMOD_FOREACH_PROC(BS_STRING_GEN_2)
        };
        bs_queryProcedures(procedures, sizeof(procedures) / sizeof(*procedures), bsmod.bsgfx, &_bsmod_procs);
    }

    bs_configureSource(BS_OBJECT_IMAGE, BSMOD_IMAGES_COUNT, (const char* []) { BSMOD_IMAGE_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_SAMPLER, BSMOD_SAMPLERS_COUNT, (const char* []) { BSMOD_SAMPLER_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_BUFFER, BSMOD_BUFFERS_COUNT, (const char* []) { BSMOD_BUFFER_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_QUEUE, BSMOD_QUEUES_COUNT, (const char* []) { BSMOD_QUEUE_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_BATCH, BSMOD_BATCHES_COUNT, (const char* []) { BSMOD_BATCH_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_RENDERER, BSMOD_RENDERERS_COUNT, (const char* []) { BSMOD_RENDERER_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_RAY_TRACER, BSMOD_RAY_TRACERS_COUNT, (const char* []) { BSMOD_RAY_TRACER_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_FONT, BSMOD_FONTS_COUNT, (const char* []) { BSMOD_FONT_IDS(BS_STRING_GEN) });
    bs_configureSource(BS_OBJECT_ATLAS, BSMOD_ATLASES_COUNT, (const char* []) { BSMOD_ATLAS_IDS(BS_STRING_GEN) });
}

void bsmod_onLateIni() { // ugly, called after first track
    bsmod_savePackage(BSMOD_CONTENT_PATH);
    bsmod_savePackage(BSGFX_CONTENT_PATH);
    bsmod_savePackage(bsmod_applicationContentPath());

    bs_except(BSX_NOT_FOUND);
    bsmod.package = bs_loadPackage(BSMOD_CONTENT_PATH);
    bs_except(0);

    if (bs_args()->track_changes)
        CreateThread(NULL, 0, bsmod_tickAsync, NULL, 0, NULL);
}

void bsmod_onCreateQuadSubtypes(bs_Range range) {
    bs_Batch* batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_QUAD_INSTANCED)->batch;
    _bsmod_subtypes[BSMOD_SUBTYPE_MATERIAL_ICON] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, batch, 0, range);
    _bsmod_subtypes[BSMOD_SUBTYPE_PRIMITIVE_ICON] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, batch, 0, range);
    _bsmod_subtypes[BSMOD_SUBTYPE_PREFAB_ICON] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, batch, 0, range);
    _bsmod_subtypes[BSMOD_SUBTYPE_BILLBOARD] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, batch, 0, range);
}

void bsmod_bindAtlases() {
    bs_Sampler* nearest_sampler = bs_fetch(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST)->sampler;

    if (bs_exists(BSMOD_ATLASES, BSMOD_ATLAS_UI)) {
        bs_Atlas* ui = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_UI)->atlas;
        bs_bindImages(BSMOD_SET_IMAGE_ATLAS_UI, BSMOD_BINDING_IMAGE_ATLAS_UI, &(bs_ImageDescriptor) {
            .image = ui->image,
            .sampler = nearest_sampler,
            .layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        }, 1);
    }

    bs_ImageDescriptor icon_atlases[BSMOD_ATLAS_ICONS_COUNT] = { 0 };

    if (bs_exists(BSMOD_ATLASES, BSMOD_ATLAS_MATERIAL_ICONS))
        icon_atlases[BSMOD_ATLAS_ICONS_MATERIALS_BINDING].image = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_MATERIAL_ICONS)->atlas->image;

    if (bs_exists(BSMOD_ATLASES, BSMOD_ATLAS_PRIMITIVE_ICONS))
        icon_atlases[BSMOD_ATLAS_ICONS_PRIMITIVES_BINDING].image = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_PRIMITIVE_ICONS)->atlas->image;

    if (bs_exists(BSMOD_ATLASES, BSMOD_ATLAS_PREFAB_ICONS))
        icon_atlases[BSMOD_ATLAS_ICONS_PREFABS_BINDING].image = bs_fetch(BSMOD_ATLASES, BSMOD_ATLAS_PREFAB_ICONS)->atlas->image;

    for (int i = 0; i < BSMOD_ATLAS_ICONS_COUNT; i++) {
        icon_atlases[i].layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL;
        icon_atlases[i].sampler = nearest_sampler;
    }

    bool bind_icons = true;
    for (int i = 0; i < BSMOD_ATLAS_ICONS_COUNT; i++) {
        if (!icon_atlases[i].image) {
            bs_warnF("Could not bind atlas icons, binding \"%d\" does not exist\n", i);
            bind_icons = false;
            break;
        }
    }

    if (bind_icons)
        bs_bindImages(BSMOD_SET_IMAGE_ATLAS_ICONS, BSMOD_BINDING_IMAGE_ATLAS_ICONS, icon_atlases, BSMOD_ATLAS_ICONS_COUNT);
}

void bsmod_onLoad() {
    bs_Sampler* nearest_sampler = bs_fetch(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST)->sampler;

    bs_Object* font = bs_loadFont(BS_FONT(BSGFX_FONTS, BSGFX_FONT_ARIAL_16, 0), bsmod.package, "fonts/consolas_16", BSGFX_ALPHABET_DEFAULT, 32.0, 0);
    if (font)
        bs_bindFont(font->font, bs_fetch(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST)->sampler, BSGFX_SET_FONTS, BSGFX_BINDING_FONT_ARIAL);

    bs_Range range = {
		.batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_QUAD_INSTANCED)->batch,
		.num = 6, // single quad
	};
	_bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, range.batch, 0, range);

    bs_Object* batch = bs_batch(BS_BATCH(BSMOD_BATCHES, BSMOD_BATCH_TILE, 0), sizeof(int), $vs_bsgfx_tile_static(), BS_BATCH_KEEP_DATA);
    if (batch && !bs_batchIsPushed(batch->batch)) {
        bsgfx_pushTile(batch->batch, bs_quad(BS_V3(0, 0, 0), BS_V2(1, 1)), BS_V3(0, 0, 0), 0, 0);
        bs_pushBatch(batch->batch, BS_U32_MAX, BS_U32_MAX);
    }

    //_bsmod_subtypes[BSMOD_SUBTYPE_SPHERE_HIGH_QUALITY] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, BSGFX_BATCH_MESH_INSTANCED, BSGFX_SUBTYPE_HAS_SHADOWS, sphere_high_quality_range);

    bs_queue(BS_QUEUE(BSMOD_QUEUES, BSMOD_QUEUE_GRAPHICS, BS_OBJECT_HAS_SWAPS_BIT), BS_QUEUE_GRAPHICS_BIT)->queue;
    bs_queue(BS_QUEUE(BSMOD_QUEUES, BSMOD_QUEUE_GRAPHICS_RASTERIZATION, 0), BS_QUEUE_GRAPHICS_BIT | BS_QUEUE_DONT_SIGNAL)->queue;

    bs_Object* renderer = bs_renderer(BS_RENDERER(BSMOD_RENDERERS, BSMOD_RENDERER, BS_OBJECT_HAS_SWAPS_BIT), 0);
    bs_ivec2 resolution = bs_resolution();
    if (renderer) {
        bs_Image* depth = bs_image(BS_IMAGE(BSMOD_IMAGES, BSMOD_IMAGE_DEPTH, 0), resolution, 0, BS_FORMAT_D32_SFLOAT_S8_UINT, BS_IMAGE_ATTACHMENT_BIT)->image;
        bs_Image* color = bs_image(BS_IMAGE(BSMOD_IMAGES, BSMOD_IMAGE_COLOR, 0), resolution, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;
        // bs_U32 subpass, bs_Image* image, bs_ImageLayout old_layout, bs_ImageLayout new_layout, bs_OutputFlags flags
        bs_output(renderer->renderer, (bs_Output) {
            .subpass = 0, 
            .image = bs_swapchain()->image->image,
            .load_op = BS_LOAD_OP_CLEAR,
            .store_op = BS_STORE_OP_STORE,
            .old_layout = BS_LAYOUT_UNDEFINED,
            .new_layout = BS_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
        });

        bs_Image* subpass_0_outputs[] = {
            depth,
        };

        for (int i = 0; i < sizeof(subpass_0_outputs) / sizeof(*subpass_0_outputs); i++) {
            bs_output(renderer->renderer, (bs_Output) {
                .subpass = 0,
                .image = subpass_0_outputs[i],
                .load_op = BS_LOAD_OP_CLEAR,
                .store_op = BS_STORE_OP_STORE,
                .old_layout = BS_LAYOUT_UNDEFINED,
                .new_layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
            });
        }

        bs_dependency(renderer->renderer, -1, 0, BS_DEPENDENCY_BY_REGION_BIT,
            BS_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | BS_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            BS_ACCESS_MEMORY_READ_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_READ_BIT | BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | BS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);

        bs_renderPass(renderer->renderer);
        bs_framebuffer(renderer->renderer, resolution);
    }

    bs_Object* renderer_3d = bs_renderer(BS_RENDERER(_bsmod_renderers, BSMOD_RENDERER_3D, BS_OBJECT_HAS_SWAPS_BIT), 0);
    if (renderer_3d) {
        bs_Image* depth = bs_image(BS_IMAGE(BSMOD_IMAGES, BSMOD_IMAGE_DEPTH_3D, 0), resolution, 0, BS_FORMAT_D32_SFLOAT_S8_UINT, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_DST_BIT)->image;
       // bs_Image* color = bs_image(BS_IMAGE(BSMOD_IMAGE_COLOR, 0), resolution, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;

        bs_output(renderer_3d->renderer, (bs_Output) {
            .subpass = 0, 
            .image = bs_swapchain()->image->image,
            .load_op = BS_LOAD_OP_LOAD,
            .store_op = BS_STORE_OP_STORE,
            .old_layout = BS_LAYOUT_COLOR_ATTACHMENT_OPTIMAL,
            .new_layout = BS_LAYOUT_PRESENT_SRC_KHR,
        });

        bs_transition(depth, 0, BS_LAYOUT_UNDEFINED, BS_LAYOUT_GENERAL);
        bs_output(renderer_3d->renderer, (bs_Output) {
            .subpass = 0, 
            .image = depth,
            .load_op = BS_LOAD_OP_LOAD,
            .store_op = BS_STORE_OP_STORE,
            .old_layout = BS_LAYOUT_GENERAL,
            .new_layout = BS_LAYOUT_GENERAL,
        });

        bs_dependency(renderer_3d->renderer, -1, 0, BS_DEPENDENCY_BY_REGION_BIT,
            BS_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | BS_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            BS_ACCESS_MEMORY_READ_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_READ_BIT | BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | BS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);

        bs_renderPass(renderer_3d->renderer);
        bs_framebuffer(renderer_3d->renderer, resolution);
    }

    bs_except(BSX_FAILED_TO_QUERY);
    bs_Object* ui = bs_loadAtlas(BS_ATLAS(BSMOD_ATLASES, BSMOD_ATLAS_UI, 0), bsmod.package, "ui", 0);
    bs_except(BSX_FAILED_TO_QUERY);
    bs_Object* material_icons = bs_loadAtlas(BS_ATLAS(BSMOD_ATLASES, BSMOD_ATLAS_MATERIAL_ICONS, 0), bsmod.package, "material_icons", 0);
    bs_except(BSX_FAILED_TO_QUERY);
    bs_Object* primitive_icons = bs_loadAtlas(BS_ATLAS(BSMOD_ATLASES, BSMOD_ATLAS_PRIMITIVE_ICONS, 0), bsmod.package, "primitive_icons", 0);
    bs_except(BSX_FAILED_TO_QUERY);
    bs_Object* prefab_icons = bs_loadAtlas(BS_ATLAS(BSMOD_ATLASES, BSMOD_ATLAS_PREFAB_ICONS, 0), bsmod.package, "prefab_icons", 0);
    bs_except(0);

    bsmod_bindAtlases();
}