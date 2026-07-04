#include <bsgfx.h>
#include <bsgfx_scene.h>
#include <UI/bsgfx_ui.h>
#include <bsgfx_instance.h>
#include <bsgfx_material.h>
#include <bsgfx_cache.h>
#include <bssteam_cloud.h>

#include <spawners/entity/bsgfx_entity.h>
#include <spawners/item/bsgfx_item.h>

#include <types/foliage/bsgfx_foliage.h>
#include <types/prefab/bsgfx_prefab.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/spawner/bsgfx_spawner.h>
#include <types/tile/bsgfx_tile.h>

#include <spawners/item/bsgfx_item.h>

#include <assert.h>

#include <basilisk-core.h>
#include <bs_log.h>

#ifdef _DEBUG
#include <bs_http.h>
#endif

int _bsgfx_subtypes[BSGFX_SUBTYPE_COUNT] = { 0 };
bsgfx_Scene _bsgfx_current_scene = { 0 };

bsgfx_Scene* bsgfx_currentScene() {
    return &_bsgfx_current_scene;
}

int _bsgfx_images = -1, _bsgfx_samplers = -1, _bsgfx_buffers = -1,
    _bsgfx_batches = -1, _bsgfx_renderers = -1, _bsgfx_ray_tracers = -1,
    _bsgfx_queues = -1, _bsgfx_atlases = -1, _bsgfx_fonts = -1;

int bsgfx_images() { return _bsgfx_images; }
int bsgfx_samplers() { return _bsgfx_samplers; }
int bsgfx_buffers() { return _bsgfx_buffers; }
int bsgfx_batches() { return _bsgfx_batches; }
int bsgfx_renderers() { return _bsgfx_renderers; }
int bsgfx_rayTracers() { return _bsgfx_ray_tracers; }
int bsgfx_queues() { return _bsgfx_queues; }
int bsgfx_atlases() { return _bsgfx_atlases; }
int bsgfx_fonts() { return _bsgfx_fonts; }

void bsgfx_createRenderers() {
   /**
    high resolution renderer
    */
    bs_Object* hi_res = bs_renderer(BS_RENDERER(BSGFX_RENDERERS, BSGFX_RENDERER_HI_RES, BS_OBJECT_HAS_SWAPS_BIT), BS_RENDERER_AUTO_RESIZE_BIT);
    if (hi_res) {
        bs_ivec2 resolution = bs_resolution();
        bs_Image* hi_res_0_depth = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_HI_RES_0_DEPTH, 0), resolution, 0, BS_FORMAT_D32_SFLOAT_S8_UINT, BS_IMAGE_ATTACHMENT_BIT)->image;

       /** subpass 0 */
        bs_output(hi_res->renderer, (bs_Output) {
            .subpass = 0,
            .image = bs_swapchain()->image->image,
            .load_op = BS_LOAD_OP_CLEAR,
            .store_op = BS_STORE_OP_STORE,
            .old_layout = BS_LAYOUT_UNDEFINED,
            .new_layout = BS_LAYOUT_PRESENT_SRC_KHR,
        });

        bs_output(hi_res->renderer, (bs_Output) {
            .subpass = 0,
            .image = hi_res_0_depth,
            .load_op = BS_LOAD_OP_CLEAR,
            .store_op = BS_STORE_OP_STORE,
            .old_layout = BS_LAYOUT_UNDEFINED,
            .new_layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        });

       /** dependencies */
        bs_dependency(hi_res->renderer, -1, 0, BS_DEPENDENCY_BY_REGION_BIT,
            BS_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT, 
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | BS_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            BS_ACCESS_MEMORY_READ_BIT, 
            BS_ACCESS_COLOR_ATTACHMENT_READ_BIT | BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | BS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);

        bs_renderPass(hi_res->renderer);
        bs_framebuffer(hi_res->renderer, bs_resolution());
    }

   /** 
    low resolution renderer 
    */
    bs_Object* lo_res = bs_renderer(BS_RENDERER(BSGFX_RENDERERS, BSGFX_RENDERER_LO_RES, BS_OBJECT_HAS_SWAPS_BIT), 0);
    if (lo_res) {
        bs_ivec2 resolution = bs_resolution();
        resolution.x /= BSGFX_PIXEL_SCALE;
        resolution.y /= BSGFX_PIXEL_SCALE;

       /** attachments */
        bs_Image* lo_res_0_depth = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_DEPTH, 0), resolution, 0, BS_FORMAT_D32_SFLOAT_S8_UINT, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_INPUT_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;
        bs_Image* lo_res_0_color = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_COLOR, 0), resolution, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_INPUT_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;
        bs_Image* lo_res_0_normal = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_NORMAL, 0), resolution, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_INPUT_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;
        bs_Image* lo_res_0_position = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_POSITION, 0), resolution, 0, BS_FORMAT_R32G32B32A32_SFLOAT, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_INPUT_ATTACHMENT_BIT)->image;
        bs_Image* lo_res_0_index = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_INDEX, 0), resolution, 0, BS_FORMAT_R32_UINT, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT | BS_IMAGE_INPUT_ATTACHMENT_BIT)->image;
        bs_Image* lo_res_0_flags = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_FLAGS, 0), resolution, 0, BS_FORMAT_R32_UINT, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT | BS_IMAGE_INPUT_ATTACHMENT_BIT)->image;
        bs_Image* lo_res_0_position_local = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_VERTEX, 0), resolution, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_INPUT_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;
        bs_Image* lo_res_1_color = bs_image(BS_IMAGE(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_1_COLOR, 0), resolution, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;

        bs_Image* subpass_0_outputs[] = {
            [BSGFX_LO_SUBPASS_0_OUT_COLOR] = lo_res_0_color,
            [BSGFX_LO_SUBPASS_0_OUT_NORMAL] = lo_res_0_normal,
            [BSGFX_LO_SUBPASS_0_OUT_INDEX] = lo_res_0_index,
            [BSGFX_LO_SUBPASS_0_OUT_FLAGS] = lo_res_0_flags,
            [BSGFX_LO_SUBPASS_0_OUT_POSITION] = lo_res_0_position,
#ifdef _DEBUG
            [BSGFX_LO_SUBPASS_0_OUT_POSITION_LOCAL] = lo_res_0_position_local,
#endif
            lo_res_0_depth,
        };

        int subpass_0_inputs[] = {
            [BSGFX_LO_SUBPASS_0_IN_COLOR] = BSGFX_LO_SUBPASS_0_OUT_COLOR,
            [BSGFX_LO_SUBPASS_0_IN_NORMAL] = BSGFX_LO_SUBPASS_0_OUT_NORMAL,
            [BSGFX_LO_SUBPASS_0_IN_INDEX] = BSGFX_LO_SUBPASS_0_OUT_INDEX,
            [BSGFX_LO_SUBPASS_0_IN_FLAGS] = BSGFX_LO_SUBPASS_0_OUT_FLAGS,
            [BSGFX_LO_SUBPASS_0_IN_POSITION] = BSGFX_LO_SUBPASS_0_OUT_POSITION,
        };

       /** subpass 0 */
        // todo only general in debug

        for (int i = 0; i < sizeof(subpass_0_outputs) / sizeof(*subpass_0_outputs); i++) {
            bs_output(lo_res->renderer, (bs_Output) {
                .subpass = 0,
                .image = subpass_0_outputs[i],
                .load_op = BS_LOAD_OP_CLEAR,
                .store_op = BS_STORE_OP_STORE,
                .old_layout = BS_LAYOUT_UNDEFINED,
                .new_layout = BS_LAYOUT_GENERAL,
            });
        }

        for (int i = 0; i < sizeof(subpass_0_inputs) / sizeof(*subpass_0_inputs); i++) {
            bs_input(lo_res->renderer, (bs_Input) {
                .subpass = 0, 
                .parent_subpass = 0, 
                .attachment = subpass_0_inputs[i],
                .flags = BS_INPUT_AWAIT_WRITE
            });
        }

        /** subpass 1 */
        int subpass_1_inputs[] = {
            [BSGFX_LO_SUBPASS_1_IN_COLOR] = BSGFX_LO_SUBPASS_0_OUT_COLOR,
            [BSGFX_LO_SUBPASS_1_IN_NORMAL] = BSGFX_LO_SUBPASS_0_OUT_NORMAL,
            [BSGFX_LO_SUBPASS_1_IN_INDEX] = BSGFX_LO_SUBPASS_0_OUT_INDEX,
            [BSGFX_LO_SUBPASS_1_IN_FLAGS] = BSGFX_LO_SUBPASS_0_OUT_FLAGS,
            [BSGFX_LO_SUBPASS_1_IN_POSITION] = BSGFX_LO_SUBPASS_0_OUT_POSITION,
        };

        for (int i = 0; i < sizeof(subpass_1_inputs) / sizeof(*subpass_1_inputs); i++) {
            bs_input(lo_res->renderer, (bs_Input) {
                .subpass = 1, 
                .parent_subpass = 0, 
                .attachment = subpass_1_inputs[i], 
                .flags = BS_INPUT_AWAIT_WRITE
            });
        }

        bs_output(lo_res->renderer, (bs_Output) {
            .subpass = 1,
            .image = lo_res_1_color,
            .load_op = BS_LOAD_OP_CLEAR,
            .store_op = BS_STORE_OP_STORE,
            .old_layout = BS_LAYOUT_UNDEFINED,
            .new_layout = BS_LAYOUT_GENERAL,
        });

        /** dependencies */
        bs_dependency(lo_res->renderer, -1, 0, BS_DEPENDENCY_BY_REGION_BIT,
            BS_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | BS_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT,
            BS_ACCESS_MEMORY_READ_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_READ_BIT | BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | BS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT);

        // this throws a validation error but the entire engine depends on it (only when developing though)
        // some bitching about BS_DEPENDENCY_BY_REGION_BIT yadayadayada
        bs_dependency(lo_res->renderer, 0, 0, 0,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT | BS_PIPELINE_STAGE_LATE_FRAGMENT_TESTS_BIT,
            BS_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT | BS_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT,
            BS_ACCESS_SHADER_READ_BIT);

        bs_dependency(lo_res->renderer, 1, 1, 0,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            BS_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            BS_ACCESS_SHADER_READ_BIT);

        bs_dependency(lo_res->renderer, 0, 1, BS_DEPENDENCY_BY_REGION_BIT,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            BS_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            BS_ACCESS_SHADER_READ_BIT);

        bs_dependency(lo_res->renderer, 0, -1, BS_DEPENDENCY_BY_REGION_BIT,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            BS_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_READ_BIT | BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            BS_ACCESS_MEMORY_READ_BIT);

        bs_dependency(lo_res->renderer, 1, -1, BS_DEPENDENCY_BY_REGION_BIT,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            BS_PIPELINE_STAGE_BOTTOM_OF_PIPE_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_READ_BIT | BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            BS_ACCESS_MEMORY_READ_BIT);

        bs_except(BSX_VALIDATION); // vulkan just bitching (this is gonna backfire like hell)
        bs_renderPass(lo_res->renderer);
        bs_except(0);
        bs_framebuffer(lo_res->renderer, resolution);

        bs_buffer(BS_BUFFER(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_CURSOR_READS, false), 2 * lo_res->renderer->num_outputs * 4,
            BS_BUFFER_USAGE_STORAGE_BUFFER_BIT | BS_BUFFER_USAGE_TRANSFER_DST_BIT | BS_BUFFER_USAGE_TRANSFER_SRC_BIT,
            BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            0);
        bs_buffer(BS_BUFFER(BSGFX_BUFFERS, BSGFX_BUFFER_LO_RES_SCREENSHOT, false), BSGFX_SCREENSHOT_DIMENSIONS.x * BSGFX_SCREENSHOT_DIMENSIONS.y * lo_res->renderer->num_outputs * 4,
            BS_BUFFER_USAGE_STORAGE_BUFFER_BIT | BS_BUFFER_USAGE_TRANSFER_DST_BIT | BS_BUFFER_USAGE_TRANSFER_SRC_BIT,
            BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            0);
    }
}

static void bsgfx_loadResources() {
    _bsgfx_queues;
    bs_queue(BS_QUEUE(BSGFX_QUEUES, BSGFX_QUEUE_GRAPHICS, BS_OBJECT_HAS_SWAPS_BIT), BS_QUEUE_GRAPHICS_BIT)->queue;
    bs_queue(BS_QUEUE(BSGFX_QUEUES, BSGFX_QUEUE_COMPUTE, BS_OBJECT_HAS_SWAPS_BIT), BS_QUEUE_COMPUTE_BIT)->queue;

    bs_sampler(BS_SAMPLER(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST, 0), BS_FILTER_NEAREST, 0);
    bs_sampler(BS_SAMPLER(BSGFX_SAMPLERS, BSGFX_SAMPLER_LINEAR, 0), BS_FILTER_LINEAR, 0);

    bs_Object* joints_buffer = bs_buffer(BS_BUFFER(BSGFX_BUFFERS, BSGFX_BUFFER_JOINTS, false), 
        1024 * sizeof(bs_mat4), 
        BS_BUFFER_USAGE_UNIFORM_BUFFER_BIT,
        BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0);

    if (joints_buffer) {
        _bsgfx_shader_joints = bs_mapBuffer(joints_buffer->buffer, BS_U32_MAX);
        bs_bindBuffer(BSGFX_SET_JOINTS, BSGFX_BINDING_JOINTS, joints_buffer->buffer);
    }

    bsgfx_iniInstances();
    
    // batches
    bs_Object* screen_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_SCREEN, 0), sizeof(bs_U32), $vs_bsgfx_color_percentage(), 0);
    bs_Object* volume_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_VOLUME_COMPUTED, BS_OBJECT_HAS_SWAPS_BIT), 0, $vs_bsgfx_volume(), 0);
    bs_Object* prefab_volume_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_PREFAB_SHADOWS, 0), 0, $vs_bsgfx_volume(), 0);
    bs_Object* volume_screen_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_VOLUME_SCREEN, 0), sizeof(bs_U32), $vs_bsgfx_color_percentage(), 0);
    bs_Object* line_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_LINE_INSTANCED, 0), sizeof(bs_U32), $vs_bsgfx_line_instanced(), 0);
    bs_Object* point_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_POINT_INSTANCED, 0), sizeof(bs_U32), $vs_bsgfx_point_instanced(), 0);
    bs_Object* sphere_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_SPHERE_INSTANCED, 0), sizeof(bs_U32), $vs_bsgfx_mesh_instanced(), 0);
    bs_Object* quad_instance_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_QUAD_INSTANCED, 0), sizeof(bs_U32), $vs_bsgfx_quad_instanced(), 0);
    bs_Object* mesh_instance_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED, 0), sizeof(bs_U32), $vs_bsgfx_mesh_instanced(), 0);
    bs_Object* bone_instance_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_BONE_INSTANCED, 0), sizeof(bs_U32), $vs_bsgfx_bone_instanced(), BS_BATCH_RAY_TRACEABLE);

    if (mesh_instance_batch && !bs_batchIsPushed(mesh_instance_batch)) {
        bsgfx_instanceType(BSGFX_INSTANCE_TYPE_MESH, BSGFX_MESH_INSTANCE_COUNT, BSGFX_SET_MESH_INSTANCES, BSGFX_BINDING_MESH_INSTANCES);
        bsgfx_instanceType(BSGFX_INSTANCE_TYPE_MESH_STATIC, BSGFX_MESH_STATIC_INSTANCE_COUNT, BSGFX_SET_MESH_STATIC_INSTANCES, BSGFX_BINDING_MESH_STATIC_INSTANCES);
    }

    if (sphere_batch && !bs_batchIsPushed(sphere_batch->batch)) {
        _bsgfx_subtypes[BSGFX_SUBTYPE_CONE_MESH] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, sphere_batch->batch, 0, bs_pushCone(sphere_batch->batch, 10, 2.0, 1.0, BS_WHITE));
        _bsgfx_subtypes[BSGFX_SUBTYPE_SPHERE_MESH] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, sphere_batch->batch, 0, bs_pushSphere(sphere_batch->batch, bs_v3V1(0), 1.0, 8, 8, BS_WHITE));
    }

    if (quad_instance_batch && !bs_batchIsPushed(quad_instance_batch->batch)) {
        bsgfx_instanceType(BSGFX_INSTANCE_TYPE_QUAD, BSGFX_QUAD_INSTANCE_COUNT, BSGFX_SET_QUAD_INSTANCES, BSGFX_BINDING_QUAD_INSTANCES);
        bs_Range range = bs_pushQuad(quad_instance_batch->batch, bs_quad(bs_v3V1(0), bs_v2V1(1)), BS_WHITE);
        _bsgfx_subtypes[BSGFX_SUBTYPE_CORNER_GRADIENT] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_UI] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_UI_COLOR] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_UI_STENCIL] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_COLOR_PICKER] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_COLOR_PICKER_HUE] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_COLOR_PICKER_ALPHA] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_DITHER] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_QUAD_MATERIAL_TEXTURE] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_ATLAS_HI] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_ATLAS_PREFAB] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_ATLAS_PREFAB_TRANSPARENT] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_ATLAS] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_TILE] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_TILE_2] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_TILE_HI] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_64_HI] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_256_HI] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_QUAD, quad_instance_batch->batch, 0, range);

        if (_bsgfx_procs.bsmod_onCreateQuadSubtypes)
            _bsgfx_procs.bsmod_onCreateQuadSubtypes(range);

        bs_pushBatch(quad_instance_batch->batch, BS_U32_MAX, BS_U32_MAX);
    }

    
    if (volume_batch && volume_screen_batch && !bs_batchIsPushed(volume_batch->batch)) {
        bs_ensureBatchSize(volume_batch->batch, 0, BSGFX_NUM_SHADOW_VERTICES);
        bs_pushQuad(volume_screen_batch->batch, bs_quad(bs_v3V1(0.0), bs_v2V1(1.0)), BSGFX_SHADOW_COLOR);
        bs_pushBatch(volume_batch->batch, 0, BSGFX_NUM_SHADOW_VERTICES);
        bs_bindBuffer(BSGFX_SET_VOLUME_OUT_VERTICES, BSGFX_BINDING_VOLUME_OUT_VERTICES, volume_batch->batch->vertex_buffer->buffer);
    }

    if (screen_batch && !bs_batchIsPushed(screen_batch->batch)) {
        bs_pushQuad(screen_batch->batch, bs_quad(bs_v3V1(0.0), bs_v2V1(1.0)), BS_WHITE);
        bs_pushQuad(screen_batch->batch, bs_quad(bs_v3V1(0.0), bs_v2V1(1.0)), BS_WHITE);
       // const float offset = 0.75;
       // bs_pushQuad(screen_batch, bs_quad(bs_v3(0.125, 0.125, 0.0), bs_v2V1(offset)), BS_WHITE);
        bs_pushBatch(screen_batch->batch, BS_U32_MAX, BS_U32_MAX);
    }

    bs_Object* mesh_volume_batch = bs_batch(BS_BATCH(BSGFX_BATCHES, BSGFX_BATCH_MESH_TYPE_VOLUME_COMPUTED, true), 0, $vs_bsgfx_volume(), 0);
    if (mesh_volume_batch) {
        bs_pushBatch(mesh_volume_batch->batch, 0, BSGFX_PRE_COMPUTED_VOLUME_SIZE);
      //  bs_bindBuffer(BSGFX_SET_VOLUME_OUT_VERTICES, BSGFX_BINDING_VOLUME_OUT_VERTICES_MESH_TYPE, mesh_volume_batch->batch->vertex_buffer->buffer);
    }

    //bs_loadAllResources(BSGFX_RESOURCE_SCRIPT, 0);
    //bsgfx_loadModels(NULL, 0);
    //bsgfx_loadMinimap();
    bsgfx_allocateMaterials();

  //  bsgfx_loadMaterials();

    bs_ivec2 resolution = bs_resolution();
    resolution.x /= BSGFX_PIXEL_SCALE;
    resolution.y /= BSGFX_PIXEL_SCALE;
    bs_Image* ray_trace_output = bs_image(BS_IMAGE(-1, 0, 0), bs_iv2(resolution.x, resolution.y), 0, BS_FORMAT_R32G32B32A32_SFLOAT, BS_IMAGE_USAGE_STORAGE_BIT)->image;
    bs_transition(ray_trace_output, 0, BS_LAYOUT_UNDEFINED, BS_LAYOUT_GENERAL);
    bs_bindImage(BSGFX_SET_RAY_TRACE_OUTPUT, BSGFX_BINDING_RAY_TRACE_OUTPUT, ray_trace_output, bs_fetch(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST)->sampler, BS_LAYOUT_GENERAL);

    /*
    bs_RayTracer* ray_tracer = bs_rayTracer(BSGFX_RAY_TRACER_MAIN, 0, $rgen_main(), $rmiss_main(), NULL)->ray_tracer;
    for (int i = 0; i < bsgfx_count(BSGFX_TYPE_PRIMITIVE); i++) {
        bsgfx_Primitive* primitive = bsgfx_get(BSGFX_TYPE_PRIMITIVE, i);
        bs_Aabb aabb = {
            .min = bs_v3Add(primitive->position, bs_v3MulV1(primitive->scale, -1)),
            .max = bs_v3Add(primitive->position, primitive->scale),
        };
        bs_accelerateAabb(ray_tracer, aabb);
    }
    bs_accelerateBatch(ray_tracer, bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_TEST)->batch);
    bs_build(ray_tracer);

    bs_bindAccelerationStructure(BSGFX_SET_ACCELERATION_STRUCTURE, BSGFX_BINDING_ACCELERATION_STRUCTURE, ray_tracer);
    */

    if (bone_instance_batch && !bs_batchIsPushed(bone_instance_batch->batch)) {
        bsgfx_instanceType(BSGFX_INSTANCE_TYPE_BONE, BSGFX_BONE_INSTANCE_COUNT, BSGFX_SET_BONE_INSTANCES, BSGFX_BINDING_BONE_INSTANCES);
    }

    if (mesh_instance_batch) {
        bs_Range pyramid = bs_pushBipyramid(mesh_instance_batch->batch, bs_v3V1(0), 0.75, 1.0, BS_WHITE);
        _bsgfx_subtypes[BSGFX_SUBTYPE_BIPYRAMID] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_MESH, mesh_instance_batch->batch, BSGFX_SUBTYPE_HAS_SHADOWS, pyramid);
    }

    if (point_batch && !bs_batchIsPushed(point_batch->batch) && bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)) {
        bsgfx_instanceType(BSGFX_INSTANCE_TYPE_POINT, BSGFX_POINT_INSTANCE_COUNT, BSGFX_SET_POINT_INSTANCES, BSGFX_BINDING_POINT_INSTANCES);
        bs_except(BSX_DUPLICATE);
        _bsgfx_subtypes[BSGFX_SUBTYPE_POINT] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_POINT, point_batch->batch, 0, bs_pushPoint(point_batch->batch, bs_v3V1(0), BS_WHITE));
        bs_except(0);
    }

    if (line_batch && !bs_batchIsPushed(line_batch->batch) && bs_exists(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)) {
        bsgfx_instanceType(BSGFX_INSTANCE_TYPE_LINE, BSGFX_LINE_INSTANCE_COUNT, BSGFX_SET_LINE_INSTANCES, BSGFX_BINDING_LINE_INSTANCES);
        bs_except(BSX_DUPLICATE);
        bs_Range range = bs_pushLine(line_batch->batch, bs_v3V1(0), bs_v3(0, 1.0, 0.0), BS_WHITE);
        _bsgfx_subtypes[BSGFX_SUBTYPE_LINE_2D] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_LINE, line_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_LINE] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_LINE, line_batch->batch, 0, range);
        _bsgfx_subtypes[BSGFX_SUBTYPE_LINE_DEPTHLESS] = bsgfx_subtype(BSGFX_INSTANCE_TYPE_LINE, line_batch->batch, 0, range);
        bs_except(0);
    }

     /**
      Load level after bsgfx resources, before bindings and subtype creation
      */
    if (_bsgfx_callbacks.loadScene)
        _bsgfx_callbacks.loadScene();


    if (_bsgfx_prefab_model && bs_exists(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED)) { // _bsgfx_prefab_model is temp
        mesh_instance_batch = bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_MESH_INSTANCED);
        if (!bs_batchIsPushed(mesh_instance_batch->batch)) {
            bs_pushBatch(mesh_instance_batch->batch, BS_U32_MAX, BS_U32_MAX);
            bs_bindBuffer(BSGFX_SET_VOLUME_IN_VERTICES, BSGFX_BINDING_VOLUME_IN_VERTICES_MESH, mesh_instance_batch->batch->vertex_buffer->buffer);
            bs_bindBuffer(BSGFX_SET_VOLUME_IN_INDICES, BSGFX_BINDING_VOLUME_IN_INDICES_MESH, mesh_instance_batch->batch->index_buffer->buffer);
            //bs_bindBuffer(BSGFX_SET_VOLUME_IN_VERTICES, BSGFX_BINDING_VOLUME_IN_VERTICES_MESH_TYPE, mesh_instance_batch->batch->vertex_buffer->buffer);
            //bs_bindBuffer(BSGFX_SET_VOLUME_IN_INDICES, BSGFX_BINDING_VOLUME_IN_INDICES_MESH_TYPE, mesh_instance_batch->batch->index_buffer->buffer);
        }

    }

    if (point_batch && !bs_batchIsPushed(point_batch->batch)) {
        bs_pushBatch(point_batch->batch, BS_U32_MAX, BS_U32_MAX);
    }

    if (line_batch && !bs_batchIsPushed(line_batch->batch)) {
        bs_pushBatch(line_batch->batch, BS_U32_MAX, BS_U32_MAX);
    }

    if (bone_instance_batch && !bs_batchIsPushed(bone_instance_batch->batch)) {
        bs_pushBatch(bone_instance_batch->batch, BS_U32_MAX, BS_U32_MAX);
       // bs_bindBuffer(BSGFX_SET_VOLUME_IN_VERTICES, BSGFX_BINDING_VOLUME_IN_VERTICES_BONE, bone_instance_batch->batch->vertex_buffer->buffer);
       // bs_bindBuffer(BSGFX_SET_VOLUME_IN_INDICES, BSGFX_BINDING_VOLUME_IN_INDICES_BONE, bone_instance_batch->batch->index_buffer->buffer);
    }

    bsgfx_createRenderers();

    if (bs_exists(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST)) {
        bs_Sampler* sampler = bs_fetch(BSGFX_SAMPLERS, BSGFX_SAMPLER_NEAREST)->sampler;
        bs_Image* lo_res_color_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_COLOR)->image;
        bs_Image* lo_res_normal_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_NORMAL)->image;
        bs_Image* lo_res_index_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_INDEX)->image;
        bs_Image* lo_res_flags_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_FLAGS)->image;
        bs_Image* lo_res_vertex_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_VERTEX)->image;
        bs_Image* lo_res_depth_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_DEPTH)->image;
        bs_Image* lo_res_position_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_POSITION)->image;
        bs_Image* lo_res_result_image = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_1_COLOR)->image;

        if (lo_res_color_image) {
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_COLOR_ATTACHMENT, lo_res_color_image, sampler, BS_LAYOUT_GENERAL);
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_COLOR_INPUT, lo_res_color_image, sampler, BS_LAYOUT_GENERAL);
        }
        if (lo_res_normal_image) {
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_NORMAL_ATTACHMENT, lo_res_normal_image, sampler, BS_LAYOUT_GENERAL);
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_NORMAL_INPUT, lo_res_normal_image, sampler, BS_LAYOUT_GENERAL);
        }
        if (lo_res_index_image) {
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_INDEX_ATTACHMENT, lo_res_index_image, sampler, BS_LAYOUT_GENERAL);
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_INDEX_INPUT, lo_res_index_image, sampler, BS_LAYOUT_GENERAL);
        }
        if (lo_res_flags_image) {
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_FLAGS_ATTACHMENT, lo_res_flags_image, sampler, BS_LAYOUT_GENERAL);
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_FLAGS_INPUT, lo_res_flags_image, sampler, BS_LAYOUT_GENERAL);
        }
        if (lo_res_vertex_image) {
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_VERTEX_ATTACHMENT, lo_res_vertex_image, sampler, BS_LAYOUT_GENERAL);
        }
        if (lo_res_position_image) {
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_POSITION_ATTACHMENT, lo_res_position_image, sampler, BS_LAYOUT_GENERAL);
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_POSITION_INPUT, lo_res_position_image, sampler, BS_LAYOUT_GENERAL);
        }
        if (lo_res_depth_image) {
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_DEPTH_ATTACHMENT, lo_res_depth_image, sampler, BS_LAYOUT_GENERAL);
//            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_0_DEPTH_INPUT, lo_res_depth_image, sampler, BS_LAYOUT_GENERAL);
        }
        if (lo_res_result_image)
            bs_bindImage(BSGFX_SET_LO_RES_0, BSGFX_BINDING_LO_RES_1_COLOR_ATTACHMENT, lo_res_result_image, sampler, BS_LAYOUT_GENERAL);
    }

    bs_pushBindings();
    bs_pushDescriptors();
    // bsgfx_preComputeInstanceVolumes();
}

void bsgfx_loadScene(const char* name) {
    bs_logSectionF("Scene \"%s\"", name);

    _bsgfx_current_scene = (bsgfx_Scene){
        .name = name,
        .name_hash = bs_stringHash(name),
    };

    bs_Scope scope = *bs_getScope();
    bs_setScope(&(bs_Scope) { 0 });

	bs_load(bsgfx_loadResources);

    if (_bsgfx_procs.bsmod_onLoad)
        bs_runSingle(_bsgfx_procs.bsmod_onLoad);

    //bs_runSingle(bsgfx_computePrefabShadows);

    bs_logObjectDiff();
    bs_logUnchangedObjects();
    bs_logBindings();

    if (_bsgfx_prefab_model) {
        bs_Queue* queue = bs_queue(BS_QUEUE(-1, 0, 0), BS_QUEUE_COMPUTE_BIT)->queue;
        bs_enqueue(queue, bsgfx_computePrefabShadows);
        bs_stallGPU();
        //bsgfx_computePrefabShadows();

    }

   // bsgfx_despawnAllItems();
    bs_setScope(&scope);

    bs_logEndOfSection();

}