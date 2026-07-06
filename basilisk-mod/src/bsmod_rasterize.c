#include <basilisk-core.h>
#include <bsgfx.h>
#include <bsgfx_scene.h>
#include <bsgfx_instance.h>
#include <bsgfx_pipeline.h>
#include <bsmod_cache.h>
#include <types/primitive/bsgfx_primitive.h>
#include <bsmod_atlas.h>

typedef struct {
    int instance_id;
    int subtype;
    int category;
    char* name;
    bs_Image* image;
    bs_Image* scaled_image;
    bs_Image* depth_image;
    bs_Buffer* buffer;
    bs_Renderer* renderer;
    bs_PipelineHash pipeline_hash;
    size_t push_constant_size;
    char push_constant[128];
} bsmod_Rasterization;

static bs_List bsmod_rasterizations = { .unit_size = sizeof(bsmod_Rasterization), .increment = 32 };
static bs_ivec2 render_size = { 0 };
static bs_ivec2 output_size = { 0 };
static void bsmod_renderToImage() {

}

void bsmod_beginRasterize(bs_ivec2 _render_size, bs_ivec2 _output_size) {
    render_size = _render_size;
    output_size = _output_size;

    bs_pushDescriptors();
    bsgfx_resetInstances();
    bsmod_rasterizations.count = 0;
}

void bsmod_rasterizeInstance(
    bs_PipelineHash pipeline_hash,
    int subtype, 
    int instance_id, 
    int category,
    char* name, 
    int width, 
    int height, 
    size_t push_constant_size, 
    unsigned char* push_constant) 
{
    bsmod_Rasterization* rasterization = bs_pushBack(&bsmod_rasterizations, &(bsmod_Rasterization) {
        .instance_id = instance_id,
        .subtype = subtype,
        .pipeline_hash = pipeline_hash,
        .name = strdup(name),
        .category = category,
        .push_constant_size = push_constant_size,
    });

    memcpy(rasterization->push_constant, push_constant, push_constant_size);

    rasterization->renderer = bs_renderer(BS_RENDERER(-1, 0, 0), 0)->renderer;
    rasterization->image = bs_image(BS_IMAGE(-1, 0, 0), render_size, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;
    rasterization->scaled_image = bs_image(BS_IMAGE(-1, 0, 0), output_size, 0, BS_FORMAT_R8G8B8A8_UNORM, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT | BS_IMAGE_USAGE_TRANSFER_DST_BIT)->image;
    rasterization->depth_image = bs_image(BS_IMAGE(-1, 0, 0), render_size, 0, BS_FORMAT_D32_SFLOAT, BS_IMAGE_ATTACHMENT_BIT | BS_IMAGE_USAGE_TRANSFER_SRC_BIT)->image;
    rasterization->buffer = bs_buffer(BS_BUFFER(-1, 0, 0), rasterization->scaled_image->dim.x * rasterization->scaled_image->dim.y * 4,
        BS_BUFFER_USAGE_STORAGE_BUFFER_BIT | BS_BUFFER_USAGE_TRANSFER_DST_BIT | BS_BUFFER_USAGE_TRANSFER_SRC_BIT,
        BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT, 0)->buffer;
    bs_transition(rasterization->image, 0, BS_LAYOUT_UNDEFINED, BS_LAYOUT_GENERAL);
    bs_transition(rasterization->scaled_image, 0, BS_LAYOUT_UNDEFINED, BS_LAYOUT_GENERAL);

    bs_output(rasterization->renderer, (bs_Output) {
        .subpass = 0,
        .image = rasterization->image,
        .old_layout = BS_LAYOUT_UNDEFINED,
        .new_layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .flags = 0
    });

    bs_output(rasterization->renderer, (bs_Output) {
        .subpass = 0,
        .image = rasterization->depth_image,
        .old_layout = BS_LAYOUT_UNDEFINED,
        .new_layout = BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL,
        .flags = 0
    });

    rasterization->renderer->dim = rasterization->image->dim;
}

void bsmod_endRasterize() {
    bs_Buffer* metadata_buffer = bs_fetch(BSGFX_BUFFERS, BSGFX_BUFFER_INSTANCE_METADATA)->buffer;
    bsgfx_InstanceMetadata* metadata = bs_bufferMap(metadata_buffer);

    bsgfx_tickInstances();
    bs_pushDescriptors();

    for (int i = 0; i < bsmod_rasterizations.count; i++) {
        bsmod_Rasterization* rasterization = bs_fetchUnit(&bsmod_rasterizations, i);

        assert(rasterization->subtype >= 0);

        bs_beginRender(rasterization->renderer);

        struct bsgfx_InstanceSubtypeMetadata* subtype_metadata = metadata->instance_subtypes + rasterization->subtype;

        int index_offset = subtype_metadata->index_offset;
        int index_count = subtype_metadata->index_count;

        bs_Batch* batch = bs_fetch(subtype_metadata->batch_source_id, subtype_metadata->batch_id);
        bs_Pipeline* pipeline = bs_pipeline(&rasterization->pipeline_hash);

        bs_clearDepth(0, rasterization->depth_image->dim, 1.0);
        bs_pushConstant(pipeline, 0, rasterization->push_constant_size, rasterization->push_constant);
        bs_render(batch, pipeline, index_offset, index_count, rasterization->instance_id, 1);

        bs_endRender(rasterization->renderer);
        
        bs_blit((bs_BlitOperation) {
            .source = rasterization->image, 
            .source_layout = BS_LAYOUT_GENERAL, 
            .source_scale = rasterization->image->dim, 
            .destination = rasterization->scaled_image, 
            .destination_layout = BS_LAYOUT_GENERAL, 
            .destination_scale = rasterization->scaled_image->dim,
        });

        bs_except(BSX_OUT_OF_BOUNDS);
        bs_copyImageToBufferAsync(
            rasterization->scaled_image,
            rasterization->buffer,
            0,
            BS_LAYOUT_GENERAL,
            0,
            bs_iv2(0, 0),
            rasterization->scaled_image->dim);
        bs_except(0);
    }
}

static struct {
    const char* package;
    const char* name;
} bsmod_queued_rasterization = { 0 };

void bsmod_queueRasterize(const char* package, const char* name, bs_Callback callback) {
    bs_Queue* post_queue = bs_fetch(_bsmod_queues, BSMOD_QUEUE_GRAPHICS_RASTERIZATION)->queue;
    bs_enqueue(post_queue, callback);

    bs_except(BSX_DEVICE_LOST);
    //bs_stall(post_queue);
    if (bs_caught())
        bsgfx_onDeviceLost();

    bsmod_queued_rasterization.package = package;
    bsmod_queued_rasterization.name = name;
}

void bsmod_pollRasterizer() {
    bs_Queue* post_queue = bs_fetch(_bsmod_queues, BSMOD_QUEUE_GRAPHICS_RASTERIZATION)->queue;

    if (bs_poll(post_queue)) {
        bsmod_AtlasPacker packer = bsmod_createAtlasPacker();

        for (int i = 0; i < bsmod_rasterizations.count; i++) {
            bsmod_Rasterization* rasterization = bs_fetchUnit(&bsmod_rasterizations, i);

            char* map = bs_mapBuffer(rasterization->buffer, BS_U32_MAX);
            // bs_savePngF(map, bs_iv2(rasterization->scaled_image->dim.x, rasterization->scaled_image->dim.y), BS_PNG_RGBA, "test%d.png", i);

            bsmod_packAtlasTexture(&packer, rasterization->name, map, rasterization->scaled_image->dim.x, rasterization->scaled_image->dim.y, rasterization->category);
        }

        bsmod_packAtlas(&packer, 2048, 2048, bsmod_queued_rasterization.package, bsmod_queued_rasterization.name);
    }
}