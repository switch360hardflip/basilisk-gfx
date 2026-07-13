#include <bsgfx.h>
#include <bsmod.h>
#include <bsmod_cache.h>
#include <bsmod_pipeline.h>
#include <bsgfx_pipeline.h>
#include <types/prefab/bsgfx_prefab.h>
#include <types/primitive/bsgfx_primitive.h>
#include <ui/bsgfx_ui.h>
#include <ui/material/bsmod_ui_material.h>
#include <ui/primitive/bsmod_ui_primitive.h>
#include <ui/prefab/bsmod_ui_prefab.h>
#include <bsmod_rasterize.h>

#define BSGFX_TRANSPARENT_OPTIONS                                   \
    .src_alpha_factor = BS_BLEND_FACTOR_SRC_ALPHA,                  \
    .dst_alpha_factor = BS_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,        \
    .src_color_factor = BS_BLEND_FACTOR_SRC_ALPHA,                  \
    .dst_color_factor = BS_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA,        \
    .color_op = BS_BLEND_OP_ADD,                                    \
    .alpha_op = BS_BLEND_OP_ADD

static void bsmod_renderDepthlessLines() {
    bs_beginComment("[BSMOD] Lines (Depthless)");
    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_line_instanced(),
            $fs_bsgfx_color(),
        },
        .topology_type = BS_TOPOLOGY_LINE_LIST,
        .skip_depth_test = true,
    });

    bs_setLineWidth(4.0);
    bs_pushConstant(pipeline, 0, sizeof(poser()->camera.result), &poser()->camera.result);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_LINE_DEPTHLESS], pipeline);
    bs_setLineWidth(1.0);
    bs_endComment();
}

static void bsmod_renderLines() {
    bs_beginComment("[BSMOD] Lines");
    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_line_instanced(),
            $fs_bsgfx_color(),
        },
        .topology_type = BS_TOPOLOGY_LINE_LIST,
    });

    bs_setLineWidth(4.0);
    bs_pushConstant(pipeline, 0, sizeof(poser()->camera.result), &poser()->camera.result);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_LINE], pipeline);
    bs_setLineWidth(1.0);
    bs_endComment();
}

static void bsmod_renderPoints() {
    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_point_instanced(),
            $fs_bsgfx_color(),
        },
        .topology_type = BS_TOPOLOGY_POINT_LIST,
        .skip_depth_test = true,
    });

    bs_pushConstant(pipeline, 0, sizeof(poser()->camera.result), &poser()->camera.result);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_POINT], pipeline);
}

static void bsmod_renderCones() {
    bs_beginComment("Cones");

    bs_except(BSX_NOT_FOUND);
    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
    if (bs_except(0))
        return;

    struct {
        bs_mat4 camera;
        bs_vec4 uv;
    } mesh_push_const = {
        .camera = poser()->camera.result,
        .uv = bs_atlasCoordinates(atlas, bs_queryAtlas(atlas, "white"), 0),
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_mesh_instanced(),
            $fs_bsgfx_model(),
        },
        .skip_depth_test = true,
    });

    bs_pushConstant(pipeline, 0, sizeof(mesh_push_const), &mesh_push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_CONE_MESH], pipeline);
}

static void bsmod_renderPreviousPass() {
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
        .inv_proj = bs_m4Inverse(poser()->camera.proj),
        .selected_color = BS_V3(1.0, 1.0, 1.0),
        .elapsed = bs_elapsedTime(),
        .light_direction = poser()->sun_direction,
        .resolution = bs_v2MulV1(bs_v2Iv2(bs_resolution()), 0.5),
    };

    //  bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_SCREEN)->batch, pipeline, 6, 6, 0, 1);
}

static void bsmod_renderUIPost() {
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
        .inv_proj = bs_m4Inverse(poser()->camera.proj),
        .selected_color = BS_V3(1.0, 1.0, 1.0),
        .elapsed = bs_elapsedTime(),
        .light_direction = poser()->sun_direction,
        .resolution = bs_v2MulV1(bs_v2Iv2(bs_resolution()), 0.5),
    };

    //  bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bs_render(bs_fetch(BSGFX_BATCHES, BSGFX_BATCH_SCREEN)->batch, pipeline, 6, 6, 0, 1);
}


static void bsmod_renderSelectedTile() {
    bs_beginComment("Selected Tiles");
    bs_PipelineHash hash;
    struct {
        bs_mat4 camera;
    } push_const = {
        .camera = poser()->camera.result,
    };

    hash = (bs_PipelineHash){
        .shaders = {
            $vs_bsgfx_tile_static(),
            $fs_bsgfx_tile_selected(),
        },
        .skip_depth_test = true,
    };
    bs_Pipeline* static_pipeline = bs_pipeline(&hash);

    bs_pushConstant(static_pipeline, 0, sizeof(push_const), &push_const);
    bs_render(bs_fetch(BSMOD_BATCHES, BSMOD_BATCH_TILE)->batch, static_pipeline, 0, BS_U32_MAX, 0, 1);

    bs_endComment();
}

static void bsmod_renderTiles() {
    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_instanced(),
            $fs_bsgfx_tile_screen(),
        },
        BSGFX_TRANSPARENT_OPTIONS,
        .stencil_front = {
            .fail_op = BS_STENCIL_OP_KEEP,
            .pass_op = BS_STENCIL_OP_KEEP,
            .depth_fail_op = BS_STENCIL_OP_KEEP,
            .compare_op = BS_COMPARE_OP_EQUAL,
            .compare_mask = 0xFF,
            .write_mask = 0x00,
            .reference = 2,
        },
    });

    bs_pushConstant(pipeline, 0, sizeof(poser()->screen_camera.result), &poser()->screen_camera.result);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_TILE_ICON], pipeline);
}

static void bsmod_renderFontSubtype(int subtype, bsgfx_Id font_id, bs_Shader* fragment_shader) {
    bs_except(BSX_NOT_FOUND);
    bs_Font* font = bs_fetch(BSGFX_FONTS, font_id)->head;
    if (bs_except(0))
        return;
    bs_Shader* vertex_shader = $vs_bsgfx_quad_instanced();

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            vertex_shader,
            fragment_shader
        },
        BSGFX_TRANSPARENT_OPTIONS,
    });

    bs_pushConstant(pipeline, 0, sizeof(poser()->screen_camera.result), &poser()->screen_camera.result);
    bsgfx_renderSubtype(subtype, pipeline);
}

static void bsmod_renderPrefabOutlines() {
    bs_beginComment("Prefabs");
    bs_PipelineHash hash = {
       .shaders = {
           $vs_bsgfx_mesh_instanced(),
           $fs_bsgfx_model(),
       },
       .polygon_type = BS_POLYGON_LINE,
       // BSGFX_REQUIRED_FOR_SHADOW_VOLUMES,
        .cull_type = bsgfx_settings()->cull_backfaces ? BS_CULL_MODE_DEFAULT : BS_CULL_MODE_NONE,
        //.attachments[0].skip_write = true,
        //  .cull_type = BS_CULL_MODE_NONE,
        .skip_depth_test = true,
    };
    bs_Pipeline* mesh_pipeline = bs_pipeline(&hash);

    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;
    struct {
        bs_mat4 camera;
        bs_vec4 uv;
    } mesh_push_const = {
        .camera = poser()->camera.result,
        .uv = $BSGFX_ATLAS_ANY_white()->coords,
    };

    bs_pushConstant(mesh_pipeline, 0, sizeof(mesh_push_const), &mesh_push_const);
    bsgfx_renderPrefabs(mesh_pipeline, BSGFX_PREFAB_SUBTYPE_MESH_POLYGON_OUTLINE);
    bs_endComment();
}

static void bsmod_renderRoundedQuads() {
    bs_beginComment("Rounded Quads");
    struct {
        bs_mat4 camera;
        float elapsed;
        float padding;
        bs_vec2 resolution;
        float border_radius;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .elapsed = bs_elapsedTime(),
        .resolution = bs_v2Iv2(bs_resolution()),
        .border_radius = 5.0,
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_rounded_instanced(),
            $fs_bsgfx_atlas_rounded(),
        },
        BSGFX_TRANSPARENT_OPTIONS,

        .stencil_front = {
            .fail_op = BS_STENCIL_OP_KEEP,
            .pass_op = BS_STENCIL_OP_KEEP,
            .depth_fail_op = BS_STENCIL_OP_KEEP,
            .compare_op = BS_COMPARE_OP_EQUAL,
            .compare_mask = 0xFF,
            .write_mask = 0x00,
        },
        .stencil_back = {
            .fail_op = BS_STENCIL_OP_KEEP,
            .pass_op = BS_STENCIL_OP_KEEP,
            .depth_fail_op = BS_STENCIL_OP_KEEP,
            .compare_op = BS_COMPARE_OP_EQUAL,
            .compare_mask = 0xFF,
            .write_mask = 0x00,
        }
    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_ATLAS_ICON], pipeline);
    bs_endComment();
}

static void bsmod_renderUI() {
    bs_beginComment("UI");
    struct {
        bs_mat4 camera;
        float elapsed;
        float padding;
        bs_vec2 resolution;
        float border_radius;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .elapsed = bs_elapsedTime(),
        .resolution = bs_v2Iv2(bs_resolution()),
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_rounded_instanced(),
            $fs_bsgfx_ui(),
        },
        BSGFX_TRANSPARENT_OPTIONS,

    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_UI], pipeline);
    bs_endComment();
}

static void bsmod_renderUISolid() {
    bs_beginComment("UI (Color only)");
    struct {
        bs_mat4 camera;
        float elapsed;
        float padding;
        bs_vec2 resolution;
        float border_radius;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .elapsed = bs_elapsedTime(),
        .resolution = bs_v2Iv2(bs_resolution()),
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_rounded_instanced(),
            $fs_bsgfx_ui_color(),
        },
        BSGFX_TRANSPARENT_OPTIONS,

    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_UI_COLOR], pipeline);
    bs_endComment();
}

static void bsmod_renderUIStencil() {
    bs_beginComment("UI Stencil");
    struct {
        bs_mat4 camera;
        float elapsed;
        float padding;
        bs_vec2 resolution;
        float border_radius;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .elapsed = bs_elapsedTime(),
        .resolution = bs_v2Iv2(bs_resolution()),
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_rounded_instanced(),
            $fs_bsgfx_ui(),
        },
        BSGFX_TRANSPARENT_OPTIONS,
        .stencil_front = {
            .fail_op = BS_STENCIL_OP_KEEP,
            .pass_op = BS_STENCIL_OP_KEEP,
            .depth_fail_op = BS_STENCIL_OP_KEEP,
            .compare_op = BS_COMPARE_OP_EQUAL,
            .compare_mask = 0xFF,
            .write_mask = 0x00,
            .reference = 2,
        },
    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_UI_STENCIL], pipeline);
    bs_endComment();
}

static void bsmod_renderDither() {
    bs_beginComment("Dither");
    struct {
        bs_mat4 camera;
        float elapsed;
        float padding;
        bs_vec2 resolution;
        float border_radius;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .elapsed = bs_elapsedTime(),
        .resolution = bs_v2Iv2(bs_resolution()),
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_rounded_instanced(),
            $fs_bsgfx_dither(),
        },
        BSGFX_TRANSPARENT_OPTIONS,

    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_DITHER], pipeline);
    bs_endComment();
}

static void bsmod_renderGradients() {
    bs_beginComment("Gradients");
    struct {
        bs_mat4 camera;
        float elapsed;
        float padding;
        bs_vec2 resolution;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .elapsed = bs_elapsedTime(),
        .resolution = bs_v2Iv2(bs_resolution()),
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_instanced(),
            $fs_bsgfx_edge_gradient(),
        },
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
    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_CORNER_GRADIENT], pipeline);
    bs_endComment();
}

static void bsmod_renderMeshSubtypes() {
    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_mesh_instanced_2d(),
            $fs_bsgfx_model(),
        },
        .stencil_front = {
            .fail_op = BS_STENCIL_OP_KEEP,
            .pass_op = BS_STENCIL_OP_KEEP,
            .depth_fail_op = BS_STENCIL_OP_KEEP,
            .compare_op = BS_COMPARE_OP_EQUAL,
            .compare_mask = 0xFF,
            .write_mask = 0x00,
            .reference = 2,
        },
    });

    struct {
        bs_mat4 camera;
        float z;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .z = 50 // todo
    };

    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderPrefabs(pipeline, BSGFX_PREFAB_SUBTYPE_MESH_HI_RES_SCREEN);
}

static void bsmod_renderMaterialTextureQuads() {
    bs_beginComment("Material Texture Quad");
    struct {
        bs_mat4 camera;
    } push_const = {
        .camera = poser()->screen_camera.result,
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_rounded_instanced(),
            $fs_bsgfx_material_texture(),
        },
        BSGFX_TRANSPARENT_OPTIONS,

    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(bsgfx_subtypes()[BSGFX_SUBTYPE_QUAD_MATERIAL_TEXTURE], pipeline);
    bs_endComment();
}

static void bsmod_renderIcons(const char* comment, int binding, int subtype) {
    bs_beginComment(comment);

    struct {
        bs_mat4 camera;
        int atlas_binding;
    } push_const = {
        .camera = poser()->screen_camera.result,
        .atlas_binding = binding,
    };

    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_rounded_instanced(),
            $fs_bsmod_material_icon(),
        },
        BSGFX_TRANSPARENT_OPTIONS,
        .stencil_front = bsmod_instance_grid_menu ? (bs_StencilOperation) {
            .fail_op = BS_STENCIL_OP_KEEP,
            .pass_op = BS_STENCIL_OP_KEEP,
            .depth_fail_op = BS_STENCIL_OP_KEEP,
            .compare_op = BS_COMPARE_OP_EQUAL,
            .compare_mask = 0xFF,
            .write_mask = 0x00,
            .reference = 2, // todo
        } : (bs_StencilOperation) { 0 },
    });
    bs_pushConstant(pipeline, 0, sizeof(push_const), &push_const);
    bsgfx_renderSubtype(subtype, pipeline);
    bs_endComment();
}

static void bsmod_renderTileIcons() {
    bs_Pipeline* pipeline = bs_pipeline(&(bs_PipelineHash) {
        .shaders = {
            $vs_bsgfx_quad_instanced(),
            $fs_bsgfx_tile_screen(),
        },
        BSGFX_TRANSPARENT_OPTIONS,
    });

    bs_pushConstant(pipeline, 0, sizeof(bs_mat4), &poser()->screen_camera.result);
    bsgfx_renderSubtype(_bsmod_subtypes[BSMOD_SUBTYPE_TILE_ICON], pipeline);
}

static void bsmod_renderPrefabs() {
    bs_beginComment("Prefabs");

    bs_PipelineHash hash = {
        .shaders = {
            $vs_bsgfx_mesh_instanced(),
            $fs_bsgfx_model(),
        },
        .cull_type = bsgfx_settings()->cull_backfaces ? BS_CULL_MODE_DEFAULT : BS_CULL_MODE_NONE,
    };
    bs_Atlas* atlas = bs_fetch(BSGFX_ATLASES, BSGFX_ATLAS_ANY)->atlas;

    struct {
        bs_mat4 camera;
        bs_vec4 uv;
    } mesh_push_const = {
        .camera = poser()->camera.result,
        .uv = bs_atlasCoordinates(atlas, bs_queryAtlas(atlas, "white"), 0),
    };

    // TODO: screenshot reimplementation
    //if (bsmod.queue.screenshot)
    //    hash.attachments[0].skip_write = false;
    bs_Pipeline* mesh_pipeline = bs_pipeline(&hash);

    bs_pushConstant(mesh_pipeline, 0, sizeof(mesh_push_const), &mesh_push_const);
    bsgfx_renderPrefabs(mesh_pipeline, BSGFX_PREFAB_SUBTYPE_MESH);
    bs_endComment();
}

void bsmod_onRenderUI() {
    bs_pushDescriptors();

    bsmod_renderPreviousPass();
    bsmod_renderDepthlessLines();
    bsmod_renderPoints();
    bsmod_renderCones();
    bsmod_renderSelectedTile();
    if (bsgfx_prefabModel())
        bsmod_renderPrefabOutlines();
    bsmod_renderGradients();
    bsmod_renderRoundedQuads();
    bsgfx_renderColorPickers();
    bsmod_renderUISolid();
    bsmod_renderUI();
    bsmod_renderUIStencil();
    bsmod_renderMaterialTextureQuads();
    bsmod_renderDither();
    bsmod_renderFontSubtype(_bsmod_subtypes[BSMOD_SUBTYPE_FONT_CONSOLAS], BSGFX_FONT_ARIAL_16, $fs_bsgfx_font_arial());
    bs_clearDepth(0, bs_fetch(BSMOD_IMAGES, BSMOD_IMAGE_DEPTH)->image->dim, 1.0);
    bsmod_renderTiles();
    if (bsgfx_prefabModel())
        bsmod_renderMeshSubtypes();
    bsgfx_renderPrimitives(poser()->screen_camera.result);

    bsmod_renderIcons("Material Icons", BSMOD_ATLAS_ICONS_MATERIALS_BINDING, _bsmod_subtypes[BSMOD_SUBTYPE_MATERIAL_ICON]);
    bsmod_renderIcons("Primitive Icons", BSMOD_ATLAS_ICONS_PRIMITIVES_BINDING, _bsmod_subtypes[BSMOD_SUBTYPE_PRIMITIVE_ICON]);
    bsmod_renderIcons("Prefab Icons", BSMOD_ATLAS_ICONS_PREFABS_BINDING, _bsmod_subtypes[BSMOD_SUBTYPE_PREFAB_ICON]);
    bsmod_renderTileIcons();
}

static void bsmod_onRender3D() {
    if (bsgfx_prefabModel()) {
        bsmod_renderPrefabs();
    }
    bsmod_renderLines();
    bsmod_renderBillboards();
}

static void bsmod_runRenderPass() {
    // bsgfx_processCursorReads();
     //bsgfx_fenceCapture();
    bs_pushDescriptors();

    bsgfx_resetInstances();
    bsmod_onTick();
    bsgfx_tickInstances();

    bs_Image* src_depth = bs_fetch(BSGFX_IMAGES, BSGFX_IMAGE_LO_RES_0_DEPTH)->image;
    bs_Image* dst_depth = bs_fetch(_bsmod_images, BSMOD_IMAGE_DEPTH_3D)->image;

    bs_blit((bs_BlitOperation) {
        .source = src_depth,
        .destination = dst_depth,
        .source_layout = BS_LAYOUT_GENERAL,
        .destination_layout = BS_LAYOUT_GENERAL,
        .source_scale = src_depth->dim,
        .destination_scale = dst_depth->dim,
    });
    bs_runPass(bs_fetch(BSMOD_RENDERERS, BSMOD_RENDERER)->renderer, bsmod_onRenderUI);

    //bs_transition(dst_depth, 0, BS_LAYOUT_PRESENT_SRC_KHR, BS_LAYOUT_GENERAL);
    bs_runPass(bs_fetch(BSMOD_RENDERERS, BSMOD_RENDERER_3D)->renderer, bsmod_onRender3D);

    //bs_Renderer* lo_res_overlay_renderer = bs_fetch(BSMOD_RENDERERS, BSMOD_RENDERER)->renderer;
    //if (lo_res_overlay_renderer) {
        //        bs_runPass(lo_res_overlay_renderer,
        //            bsgfx_loResUISubpass0,
        //            NULL);
    //}
}

bs_Queue* bsmod_onQueue() {
    bs_Queue* queue = bs_fetch(BSMOD_QUEUES, BSMOD_QUEUE_GRAPHICS)->queue;
    bs_Queue* bsgfx_queue = bs_fetch(BSGFX_QUEUES, BSGFX_QUEUE_GRAPHICS)->queue;

    bs_awaitQueue(bsgfx_queue, BS_PIPELINE_STAGE_VERTEX_INPUT_BIT);
    bs_enqueue(queue, bsmod_runRenderPass);

    bs_except(BSX_DEVICE_LOST);
    bs_stall(queue);
    if (bs_caught())
        bsgfx_onDeviceLost();

    if (bsmod_keyDownOnce(BS_KEY_G))
        bsmod_queueRasterize(BSMOD_CONTENT_PATH, "material_icons", bsmod_rasterizeMaterialIcons);

    if (bsmod_keyDownOnce(BS_KEY_K))
        bsmod_queueRasterize(BSMOD_CONTENT_PATH, "primitive_icons", bsmod_rasterizePrimitiveIcons);

    if (bsmod_keyDownOnce(BS_KEY_L))
        bsmod_queueRasterize(BSMOD_CONTENT_PATH, "prefab_icons", bsmod_rasterizePrefabIcons);

    bsmod_pollRasterizer();

    return queue;
}