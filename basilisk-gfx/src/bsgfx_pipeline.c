#include <basilisk-gfx.h>

#include <UI/bsgfx_ui.h>

#include <spawners/item/bsgfx_item.h>
#include <types/primitive/bsgfx_primitive.h>
#include <types/prefab/bsgfx_prefab.h>
#include <types/tile/bsgfx_tile.h>

#include <math.h>
#include <assert.h>

/*
#ifdef _DEBUG
#include <GFSDK_Aftermath_GpuCrashDump.h>
#endif
*/

void bsgfx_onDeviceLost() {
    /*
#ifdef _DEBUG
    GFSDK_Aftermath_CrashDump_Status status = 0;
    bsgfx_checkGFSDK(GFSDK_Aftermath_GetCrashDumpStatus(&status));

    bs_Timer timer = bs_timer();
    const double timeout_seconds = 60.0;

    while (status != GFSDK_Aftermath_CrashDump_Status_CollectingDataFailed &&
        status != GFSDK_Aftermath_CrashDump_Status_Finished &&
        timer.seconds < timeout_seconds)
    {
        Sleep(50);
        bsgfx_checkGFSDK(GFSDK_Aftermath_GetCrashDumpStatus(&status));
        bs_checkTimer(&timer);
    }
#endif
*/
    //bs_critical(BS_CONSTANT_STRING("Device lost"));
}



  /*==============================================================================
   * Compute Pipe
   *============================================================================*/

static void bsgfx_computePipe() {
    bsgfx_computeShadowVolumes();
}



  /*==============================================================================
   * Render Pipe
   *============================================================================*/

static inline int bsgfx_queryLoResImageIndex(int id) {
    bs_Renderer* renderer = bs_fetch(BSGFX_RENDERERS, BSGFX_RENDERER_LO_RES)->renderer;
    for (int i = 0; i < renderer->num_outputs; i++) {
        if (renderer->outputs[i].image->head.id == id)
            return i;
    }

    bs_throwBasilisk(BSX_FAILED_TO_QUERY);
    return -1;
}

static void bsgfx_onTileFinishedRender() {

}

static void bsgfx_shadowedGeometryPipe() {
    if (_bsgfx_callbacks_.render)
        _bsgfx_callbacks_.render();

        /**
        Primitives / Meshes
        */
    if (bs_exists(BSGFX_BATCHES, BSGFX_BATCH_PRIMITIVE_TILES)) {
        bs_beginComment("Tiles");

        bs_PipelineHash hash;
        struct {
            bs_mat4 camera;
        } push_const = {
            .camera = _poser_->camera.result,
        };

        hash = (bs_PipelineHash){
            .shaders = {
                $vs_bsgfx_tile_static(),
                $fs_bsgfx_tile(),
            },
        };
        bs_Pipeline* static_pipeline = bs_pipeline(&hash);

        bs_pushConstant(static_pipeline, 0, sizeof(push_const), &push_const);
        bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_PRIMITIVE_TILES)->batch, static_pipeline, 0, BS_U32_MAX, 0, 1);

        bs_endComment();

        bsgfx_renderPrimitives(_poser_->camera.result);
    }

    bsgfx_renderScenePrefabs();

    bs_barrier(0,
        BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
        BS_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
        BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
        BS_ACCESS_SHADER_READ_BIT);


    {
        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = {
                $vs_bsgfx_quad_instanced(),
                $fs_bsgfx_atlas(),
            },
            BSGFX_REQUIRED_FOR_SHADOW_STENCIL,
            .skip_depth_write = true
        });

        bs_beginComment(BS_CONSTANT_STRING("Atlas"));
        bs_pushConstant(pipeline, 0, sizeof(_poser_->world_camera.result), &_poser_->world_camera.result);
        bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS], pipeline);
        bs_endComment();

    }

   /**
    Foliage
    */
    if (bs_exists(BSGFX_BATCHES, BSGFX_BATCH_FOLIAGE) && _bsgfx_settings_.draw_foliage
//#ifdef _DEBUG
        // TODO: screenshot reimplementation
//        && bsmod.queue.isolated_subtype == 0
//#endif
        ) {
        bs_beginComment(BS_CONSTANT_STRING("Foliage"));
        struct {
            bs_mat4 camera;
            float elapsed;
            float padding;
            bs_vec2 resolution;
        } push_const = {
            .camera = _poser_->camera.result,
            .elapsed = bs_elapsedTime(),
            .resolution = bs_v2Iv2(bs_resolution()),
        };

        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = { 
                $vs_bsgfx_foliage(),
                $fs_bsgfx_atlas(),
            },
            .src_alpha_factor = BS_BLEND_FACTOR_ONE,
            .dst_alpha_factor = BS_BLEND_FACTOR_ZERO,
            .src_color_factor = BS_BLEND_FACTOR_SRC_ALPHA,
            .dst_color_factor = BS_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,
            .color_op = BS_BLEND_OP_ADD,
            .alpha_op = BS_BLEND_OP_ADD,
        });

        bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
        bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_FOLIAGE)->batch, pipeline, 0, BS_U32_MAX, 0, 1);
        bs_endComment();
    }

    if (_bsgfx_settings_.draw_sprites) {
        struct {
            bs_mat4 camera;
            float elapsed;
            float padding;
            bs_vec2 resolution;
        } push_const = {
            .camera = _poser_->world_camera.result,
            .elapsed = bs_elapsedTime(),
            .resolution = bs_v2Iv2(bs_resolution()),
        };

        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) { 
            .shaders = { 
                $vs_bsgfx_quad_instanced(),
                $fs_bsgfx_atlas(),
            },
            BSGFX_REQUIRED_FOR_SHADOW_STENCIL,
            .skip_depth_write = true
        });

        // bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
        // bsgfx_renderSubtype(bsgfx_querySubtype(BSGFX_INSTANCE_TYPE_QUAD_LEGACY, BSGFX_QUAD_LEGACY_KEY_SPRITE), pipeline);
    }

}

 // Stuff rendered in here will not be affected by shadows
static void bsgfx_unshadowedGeometryPipe() {
    if (_bsgfx_settings.draw_lines) {
        bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
        struct {
            bs_mat4 camera;
            bs_vec4 uv;
        } mesh_push_const = {
            .camera = _poser_->camera.result,
            .uv = bs_atlasCoordinates(atlas, bs_queryAtlas(atlas, "white"), 0),
        };

        {
            bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
                .shaders = {
                    $vs_bsgfx_point_instanced(),
                    $fs_bsgfx_color(),
                },
                .topology_type = BS_TOPOLOGY_POINT_LIST,
            });

            bs_pushConstant(pipeline, 0, sizeof(_poser_->camera.result), &_poser_->camera.result);
            bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_POINT], pipeline);
        }

        {
            bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
                .shaders = {
                    $vs_bsgfx_line_instanced(),
                    $fs_bsgfx_color(),
                },
                .topology_type = BS_TOPOLOGY_LINE_LIST,
            });

            bs_pushConstant(pipeline, 0, sizeof(_poser_->camera.result), &_poser_->camera.result);
            bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_LINE], pipeline);
        }

        {
            bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
                .shaders = {
                    $vs_bsgfx_line_instanced(),
                    $fs_bsgfx_color(),
                },
                .topology_type = BS_TOPOLOGY_LINE_LIST,
            });

            bs_pushConstant(pipeline, 0, sizeof(_poser_->screen_camera.result), &_poser_->screen_camera.result);
            bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_LINE_2D], pipeline);
        }

        {
            bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
                .shaders = {
                    $vs_bsgfx_line_instanced(),
                    $fs_bsgfx_color(),
                },
                .topology_type = BS_TOPOLOGY_LINE_LIST,
                .skip_depth_test = true,
            });

            bs_pushConstant(pipeline, 0, sizeof(_poser_->camera.result), &_poser_->camera.result);
            bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_LINE_DEPTHLESS], pipeline);
        }

        if (bs_exists(BSGFX_BATCHES, BSGFX_BATCH_SPHERE_INSTANCED)) {
            if (_bsgfx_settings.draw_spheres) {
                bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
                    .shaders = {
                        $vs_bsgfx_mesh_instanced(),
                        $fs_bsgfx_color(),
                    },
                    .topology_type = BS_TOPOLOGY_LINE_LIST,
                });

                bs_pushConstant(pipeline, 0, sizeof(mesh_push_const), &mesh_push_const);
                bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_SPHERE_MESH], pipeline);
            }

            {
                bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
                    .shaders = {
                        $vs_bsgfx_mesh_instanced(),
                        $fs_bsgfx_model(),
                    },
                    .skip_depth_test = true,
                });

                bs_pushConstant(pipeline, 0, sizeof(mesh_push_const), &mesh_push_const);
                bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_CONE_MESH], pipeline);
            }
        }
    }
}



 /*==============================================================================
  * Low Resolution Renderer
  *============================================================================*/

 /**
  Low Resolution Subpass 0
  Renders pixelated geometry
  Writes to
    BSGFX_IMAGE_LO_RES_DEPTH
    BSGFX_IMAGE_LO_RES_COLOR
    BSGFX_IMAGE_LO_RES_NORMAL
    BSGFX_IMAGE_LO_RES_POSITION
    BSGFX_IMAGE_LO_RES_INDEX
  */
static void bsgfx_loResSubpass0() {
    bs_beginComment(BS_CONSTANT_STRING("Low Resolution Subpass 0"));

    bsgfx_shadowedGeometryPipe();

    bs_beginComment(BS_CONSTANT_STRING("Fonts"));
    //bsgfx_renderFontSubtypes();
    bs_endComment();
   /**
    Zero out the stencil buffer
    This is important so it doesn't interfere with the stencil shadow volumes
    */
    bs_clearStencil(0, bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_DEPTH)->image->dim, 0);

    bsgfx_renderPrefabShadowVolumes();
    bsgfx_renderShadowVolumes();
    bsgfx_renderFineShadowVolumes();
    bsgfx_renderShadowVolumes();
    bsgfx_unshadowedGeometryPipe();

    {
        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = {
                $vs_bsgfx_bone_instanced_2d(),
                $fs_bsgfx_model(),
            },
        });

        // for (int i = 0; i < bs_resourcesCount(BS_RESOURCE_MODEL); i++) {
        //     bs_Model* model = bs_resource(BS_RESOURCE_MODEL, i)->model;
        //     bs_except(BSX_FAILED_TO_QUERY);
        //     int subtype = bsgfx_querySubtype(BSGFX_INSTANCE_TYPE_BONE, bs_stringHash(model->path) + BSGFX_MODEL_SCREEN_KEY_OFFSET);
        //     if (!bs_caught()) {
        //         bs_pushConstant(pipeline, 0, sizeof(dust->static_camera.result), &dust->static_camera.result);
        //         bsgfx_renderSubtype(subtype, pipeline);
        //     }
        // }

        // for (bsgfx_Id id = bs_first(BSGFX_CLOTHING); id <= bs_last(BSGFX_CLOTHING); id++) {
        //     bs_except(BSX_FAILED_TO_QUERY);
        //     int screen_subtype = bsgfx_querySubtype(BSGFX_INSTANCE_TYPE_BONE, BSGFX_ID_SCREEN_KEY + id);
        //     if (!bs_caught())
        //         bsgfx_renderSubtype(screen_subtype, pipeline);
        // }
    }

    {
        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = {
                $vs_bsgfx_mesh_instanced_2d(),
                $fs_bsgfx_model(),
            },
        });

        //bs_pushConstant(pipeline, 0, sizeof(poser->static_camera.result), &poser->static_camera.result);
        //bsgfx_renderPrefabs(pipeline, BSGFX_PREFAB_SUBTYPE_MESH);
       // bs_pushConstant(pipeline, 0, sizeof(_poser_->static_camera.result), &_poser_->static_camera.result);
       // bsgfx_renderPrefabPrimitives(pipeline, BSGFX_PREFAB_SUBTYPE_PRIMITIVE);
    }

    bs_endComment();
}

 /**
  Low Resolution Subpass 1
  Used for post processing
  Writes to BSGFX_IMAGE_LO_RES_RESULT
  */
static void bsgfx_loResSubpass1() {
    bs_beginComment(BS_CONSTANT_STRING("Low Resolution Subpass 1"));

    if (bs_exists(BSGFX_BATCHES, BSGFX_BATCH_SCREEN)) {
        bs_barrier(0,
            BS_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT,
            BS_PIPELINE_STAGE_FRAGMENT_SHADER_BIT,
            BS_ACCESS_COLOR_ATTACHMENT_WRITE_BIT,
            BS_ACCESS_SHADER_READ_BIT);

        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = { 
                $vs_bsgfx_color_percentage(),
                $fs_bsgfx_hilight(),
            },
        });

        struct {
            float offset_x;
            float offset_y;
            float elapsed;
         //   int noise;
        } push_const = {
            .offset_x = _poser_->world_camera.position.x / (bs_resolution().x / BSGFX_PIXEL_SCALE),
            .offset_y = _poser_->world_camera.position.y / (bs_resolution().y / BSGFX_PIXEL_SCALE),
            .elapsed = bs_elapsedTime(),
         //   .noise = bsgfx_queryTexture(512, "noise_00"),
        };

        bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
        bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_SCREEN)->batch, pipeline, 0, 6, 0, 1);
    }

    bs_endComment();
}



 /*==============================================================================
  * Low Resolution UI Renderer
  *============================================================================*/

static void bsgfx_loResUISubpass0_renderPreviousPass() {
    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_color_percentage(),
            $fs_bsgfx_lo_res_ui_post_0(),
        },
        BSGFX_TRANSPARENT_OPTIONS,
    });

    struct {
        bs_mat4 inv_proj;
        bs_vec3 selected_color;
        float elapsed;
        bs_vec3 light_direction;
        float pad;
        bs_vec2 resolution;
    } push_const = {
        .selected_color = bs_v3(1.0, 1.0, 1.0),
        .elapsed = bs_elapsedTime(),
        .light_direction = _poser_->sun_direction,
        .resolution = bs_v2Iv2(bs_resolution()),
    };
    bs_m4Inverse(&_poser_->camera.proj, &push_const.inv_proj);

  //  bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_SCREEN)->batch, pipeline, 0, 6, 0, 1);
}

static void bsgfx_loResUISubpass0() {
    bs_beginComment(BS_CONSTANT_STRING("Low Resolution UI Subpass 0"));

    //bsgfx_loResUISubpass0_renderPreviousPass();

    bs_endComment();
}



 /*==============================================================================
  * High Resolution Renderer
  *============================================================================*/

 /**
  High Resolution Subpass 0
  Writes to the swapchain (or whatever)
  */
static void bsgfx_hiResSubpass0() {
    bs_beginComment(BS_CONSTANT_STRING("High Resolution Subpass 0"));

    bs_Renderer* renderer = bs_fetch(BSGFX_RENDERERS, BSGFX_RENDERER_HI_RES)->renderer;

   // Final post processing step on the BSGFX_IMAGE_LO_RES_RESULT
    bs_beginComment("Post processing");
    if (bs_exists(BSGFX_BATCHES, BSGFX_BATCH_SCREEN)) {
         bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = { 
                $vs_bsgfx_color_percentage(),
                $fs_bsgfx_pixelation(),
            },
            BSGFX_TRANSPARENT_OPTIONS,
        });

        struct {
            bs_mat4 inv_proj;
            bs_vec3 selected_color;
            float elapsed;
            bs_vec3 light_direction;
            float pad;
            bs_vec2 resolution;
        } push_const = {
            .selected_color = bs_v3(1.0, 1.0, 1.0),
            .elapsed = bs_elapsedTime(),
            .light_direction = _poser_->sun_direction,
            .resolution = bs_v2Iv2(bs_resolution()),
        };
        bs_m4Inverse(&_poser_->camera.proj, &push_const.inv_proj);

        bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
        bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_SCREEN)->batch, pipeline, 0, 6, 0, 1);
    }
    bs_endComment();

    {
        // bs_beginComment("Thumbnail");
        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = { 
                $vs_bsgfx_quad_instanced(),
                $fs_bsgfx_level_thumbnail(),
            },
            BSGFX_TRANSPARENT_OPTIONS,
        });

       // bs_pushConstant(pipeline, 0, sizeof(poser->screen_camera.result), &poser->screen_camera.result);
       // bsgfx_renderSubtype(bsgfx_querySubtypeNull(BSGFX_INSTANCE_TYPE_QUAD_LEGACY, BSGFX_QUAD_LEGACY_KEY_LEVEL_THUMBNAIL), pipeline);
    }

    bsgfx_renderColorPickers();

    /**
     Textures
     */
    {
        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = {
                $vs_bsgfx_quad_instanced(),
                $fs_bsgfx_256_hi_res(),
            },
            BSGFX_TRANSPARENT_OPTIONS,
        });

        bs_pushConstant(pipeline, 0, sizeof(_poser_->screen_camera.result), &_poser_->screen_camera.result);
        bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_256_HI], pipeline);
    }

    {
        struct {
            bs_mat4 camera;
            float elapsed;
            float padding;
            bs_vec2 resolution;
        } push_const = {
            .camera = _poser_->screen_camera.result,
            .elapsed = bs_elapsedTime(),
            .resolution = bs_v2Iv2(bs_resolution()),
        };

        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = {
                $vs_bsgfx_quad_instanced(),
                $fs_bsgfx_atlas(),
            },
            BSGFX_TRANSPARENT_OPTIONS,
        });
        bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
        bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_ATLAS_HI], pipeline);
    }

    {
       // bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
       //     .shaders = { 
       //         $vs_bsgfx_quad_legacy_instanced(),
       //         $fs_bsgfx_tile(),
       //     },
       //     BSGFX_TRANSPARENT_OPTIONS,
       // });


       // bs_pushConstant(pipeline, 0, sizeof(poser->screen_camera.result), &poser->screen_camera.result);
       // bsgfx_renderSubtype(bsgfx_querySubtypeNull(BSGFX_INSTANCE_TYPE_QUAD_LEGACY, BSGFX_QUAD_LEGACY_KEY_TILE_HI_RES), pipeline);
    }

    {
        bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
            .shaders = {
                $vs_bsgfx_quad_instanced(),
                $fs_bsgfx_tile_screen(),
            },
            BSGFX_TRANSPARENT_OPTIONS,
        });

        bs_pushConstant(pipeline, 0, sizeof(_poser_->screen_camera.result), &_poser_->screen_camera.result);
        bsgfx_renderSubtype(_bsgfx_subtypes_[BSGFX_SUBTYPE_TILE_HI], pipeline);
    }

    bs_endComment();
}



 /*==============================================================================
  * Graphics Pipeline
  *============================================================================*/

static void bsgfx_graphicsPipe() {

    if (bs_exists(BSGFX_RENDERERS, BSGFX_RENDERER_LO_RES)) {
        bs_Renderer* lo_res_renderer = bs_fetch(BSGFX_RENDERERS, BSGFX_RENDERER_LO_RES)->renderer;
        bs_runPass(lo_res_renderer,
            bsgfx_loResSubpass0,
            bsgfx_loResSubpass1,
            NULL);
      //  bsgfx_resetInstances();
    }

    if (bs_exists(BSGFX_RAY_TRACERS, BSGFX_RAY_TRACER_MAIN)) {
        bs_RayTracer* tracer = bs_fetch(BSGFX_RAY_TRACERS, BSGFX_RAY_TRACER_MAIN)->ray_tracer;
        bs_Pipeline* ray_trace_pipeline = bs_rayTracingPipeline(&(bs_RayTracePipelineHash) {
            //   .ray_gen_shader = $rgen_main(),
            //   .miss_shader = $rmiss_main(),
            .ray_tracer = tracer,
        });
        bs_ivec2 resolution = bs_resolution();
        resolution.x /= BSGFX_PIXEL_SCALE;
        resolution.y /= BSGFX_PIXEL_SCALE;
        bs_rayTrace(tracer, ray_trace_pipeline, resolution.x, resolution.y, 1);
    }

    if (bs_exists(BSGFX_RENDERERS, BSGFX_RENDERER_HI_RES)) {
        bs_Renderer* hi_res_renderer = bs_fetch(BSGFX_RENDERERS, BSGFX_RENDERER_HI_RES)->renderer;
        bs_runPass(hi_res_renderer,
            bsgfx_hiResSubpass0,
            NULL);
        
       // bsgfx_blitMinimap();
    }

    if (_bsgfx_procs_.bsmod_copyHoveringDataToBuffer)
        _bsgfx_procs_.bsmod_copyHoveringDataToBuffer();
    //bsgfx_doCursorReads();

}

 /**
  This could be basilisk code but for now it's not
  */
static void bsgfx_swapBufferBindings() {
    bs_List* sources = bs_objectSources();

    for (int i = 0; i < BSGFX_BUFFERS_COUNT; i++) {
        bs_Buffer* buffer = bs_fetch(BSGFX_BUFFERS, i)->buffer;
        if (!buffer)
            continue;

        if (!(buffer->flags & BSI_BUFFER_IS_BOUND))
            continue;

        if (!(buffer->flags & BSI_BUFFER_SWAPS_BIT))
            continue;

        bs_bindBuffer(buffer->bind_set, buffer->binding, buffer);
    }

    for (int i = 0; i < BSGFX_BATCHES_COUNT; i++) {
        bs_Batch* batch = bs_fetch(BSGFX_BATCHES, i)->batch;
        if (!batch)
            continue;

        if (!(batch->flags & BSI_BATCH_SWAPS_BIT))
            continue;

        if (!bs_batchIsPushed(batch))
            continue;

        if (batch->vertex_buffer->buffer && batch->vertex_buffer->buffer->flags & BSI_BUFFER_IS_BOUND)
            bs_bindBuffer(batch->vertex_buffer->buffer->bind_set, batch->vertex_buffer->buffer->binding, batch->vertex_buffer->buffer);

        if (batch->index_buffer->buffer && batch->index_buffer->buffer->flags & BSI_BUFFER_IS_BOUND)
            bs_bindBuffer(batch->index_buffer->buffer->bind_set, batch->index_buffer->buffer->binding, batch->index_buffer->buffer);
    }

    bs_pushDescriptors();
}

void bsgfx_pipeline() {
    if (!bs_exists(BSGFX_QUEUES, BSGFX_QUEUE_GRAPHICS))
        return;

    bs_Queue* graphics_queue = bs_fetch(BSGFX_QUEUES, BSGFX_QUEUE_GRAPHICS)->queue;
    bs_Queue* compute_queue = bs_exists(BSGFX_QUEUES, BSGFX_QUEUE_COMPUTE) ? bs_fetch(BSGFX_QUEUES, BSGFX_QUEUE_COMPUTE)->queue : NULL;
    bs_setScope(&(bs_Scope) { 0 });

    bs_acquire();

   // bsgfx_swapBufferBindings();

    if (compute_queue) {
       // Enqueue dispatch commands and block the CPU until completion
        bs_enqueue(compute_queue, bsgfx_computePipe);
        bs_stall(compute_queue);
    }

   /** 
    Enqueue rendering commands after acquiring the swapchain image
    and awaiting the compute queue's completion.
    Blocks the CPU until completion
    */
    bs_awaitAcquisition();
    if (compute_queue)
        bs_awaitQueue(compute_queue, BS_PIPELINE_STAGE_VERTEX_INPUT_BIT);
    bs_enqueue(graphics_queue, bsgfx_graphicsPipe);

    bs_stall(graphics_queue);

    bs_Queue* bsmod_queue = NULL;
    if (_bsgfx_procs_.bsmod_onQueue)
        bsmod_queue = _bsgfx_procs_.bsmod_onQueue();

    bs_Queue* last_queue = bsmod_queue ? bsmod_queue : graphics_queue;
    bs_present(last_queue, last_queue, NULL);
    bsgfx_resetInstances();
}
