// STD
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <windows.h>
#include <winreg.h>
#include <assert.h>

#include <basilisk-core.h>
#include <bs_internal.h>
#include <vulkan.h>

static inline bs_U32 bs_queryMemoryType(bs_U32 filter, VkMemoryPropertyFlags props) {
    VkPhysicalDeviceMemoryProperties mem_props;
    vkGetPhysicalDeviceMemoryProperties(_bs_instance_->physical_device, &mem_props);

    for (bs_U32 i = 0; i < mem_props.memoryTypeCount; i++) {
        if ((filter & (1 << i)) && (mem_props.memoryTypes[i].propertyFlags & props) == props)
            return i;
    }

    bs_throwBasilisk(BSXI_INTERNAL | BSX_FAILED_TO_QUERY);
    return 0;
}

bool bs_isStencilFormat(bs_Format format) {
    return
        format == BS_FORMAT_D32_SFLOAT_S8_UINT ||
        format == BS_FORMAT_D24_UNORM_S8_UINT ||
        format == BS_FORMAT_D16_UNORM_S8_UINT;
}

bool bs_isDepthFormat(bs_Format format) {
    return bs_isStencilFormat(format) ||
        format == BS_FORMAT_D16_UNORM || format == BS_FORMAT_D32_SFLOAT;
}

bool bs_hasAlpha(bs_Format format) {
    return
        format == BS_FORMAT_R8G8B8A8_UNORM ||
        format == BS_FORMAT_R8G8B8A8_SNORM ||
        format == BS_FORMAT_R8G8B8A8_USCALED ||
        format == BS_FORMAT_R8G8B8A8_SSCALED ||
        format == BS_FORMAT_R8G8B8A8_UINT ||
        format == BS_FORMAT_R8G8B8A8_SINT ||
        format == BS_FORMAT_R8G8B8A8_SRGB ||
        format == BS_FORMAT_B8G8R8A8_UNORM ||
        format == BS_FORMAT_B8G8R8A8_SNORM ||
        format == BS_FORMAT_B8G8R8A8_USCALED ||
        format == BS_FORMAT_B8G8R8A8_SSCALED ||
        format == BS_FORMAT_B8G8R8A8_UINT ||
        format == BS_FORMAT_B8G8R8A8_SINT ||
        format == BS_FORMAT_B8G8R8A8_SRGB;
}

static inline const char* bs_layoutName(bs_ImageLayout layout) {
    switch (layout) {
        case BS_LAYOUT_UNDEFINED: return "Undefined";
        case BS_LAYOUT_GENERAL: return "General";
        case BS_LAYOUT_COLOR_ATTACHMENT_OPTIMAL: return "Color (Attachment)";
        case BS_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL: return "Depth/Stencil (Attachment)";
        case BS_LAYOUT_DEPTH_STENCIL_READ_ONLY_OPTIMAL: return "Depth/Stencil (Read Only)";
        case BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL: return "Shader (Read Only)";
        case BS_LAYOUT_TRANSFER_SRC_OPTIMAL: return "Transfer Src";
        case BS_LAYOUT_TRANSFER_DST_OPTIMAL: return "Transfer Dst";
        case BS_LAYOUT_PREINITIALIZED: return "Preinitialized";
        case BS_LAYOUT_DEPTH_READ_ONLY_STENCIL_ATTACHMENT_OPTIMAL: return "Depth (Read Only) Stencil (Attachment)";
        case BS_LAYOUT_DEPTH_ATTACHMENT_STENCIL_READ_ONLY_OPTIMAL: return "Depth/Stencil (Read Only)";
        case BS_LAYOUT_DEPTH_ATTACHMENT_OPTIMAL: return "Depth (Attachment)";
        case BS_LAYOUT_DEPTH_READ_ONLY_OPTIMAL: return "Depth (Read Only)";
        case BS_LAYOUT_STENCIL_ATTACHMENT_OPTIMAL: return "Stencil (Attachment)";
        case BS_LAYOUT_STENCIL_READ_ONLY_OPTIMAL: return "Stencil (Read Only)";
        case BS_LAYOUT_READ_ONLY_OPTIMAL: return "Read Only";
        case BS_LAYOUT_ATTACHMENT_OPTIMAL: return "Attachment";
        case BS_LAYOUT_PRESENT_SRC_KHR: return "Present Src";
    }
    return "Unknown";
}

int bs_imageSwapsCount(bs_Image* image) {
    return image->flags & BS_IMAGE_SWAPS_BIT ? _bs_settings_.frames_in_flight : 1;
}

void bs_transition(bs_Image* image, int index, bs_ImageLayout old_layout, bs_ImageLayout new_layout) {
    VkCommandBuffer commands = bsi_fetchCommands();

    if (old_layout == new_layout) return;
    if (index != 0 && index >= image->num_indices)
        bs_throwBasilisk(BSXI_INTERNAL | BSX_OUT_OF_BOUNDS);

    VkPipelineStageFlags src_stage;
    VkPipelineStageFlags dst_stage;

    VkImageMemoryBarrier barrier = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_MEMORY_BARRIER,
        .oldLayout = old_layout,
        .newLayout = new_layout,
        .srcQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .dstQueueFamilyIndex = VK_QUEUE_FAMILY_IGNORED,
        .image = image->_->vk_image,
        .subresourceRange = {
            .baseMipLevel = 0,
            .levelCount = 1,
            .baseArrayLayer = index,
            .layerCount = 1,
        }
    };

    if (bs_isDepthFormat(image->format)) {
        barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_DEPTH_BIT;
        if (bs_isStencilFormat(image->format)) {
            barrier.subresourceRange.aspectMask |= VK_IMAGE_ASPECT_STENCIL_BIT;
        }
    }
    else {
        barrier.subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT;
    }

    if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        src_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
        dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_COLOR_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_COLOR_ATTACHMENT_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dst_stage = VK_PIPELINE_STAGE_COLOR_ATTACHMENT_OUTPUT_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_DEPTH_STENCIL_ATTACHMENT_OPTIMAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_READ_BIT | VK_ACCESS_DEPTH_STENCIL_ATTACHMENT_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dst_stage = VK_PIPELINE_STAGE_EARLY_FRAGMENT_TESTS_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_GENERAL && new_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_TRANSFER_SRC_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = VK_ACCESS_TRANSFER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        src_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
        dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_UNDEFINED && new_layout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = 0;
        barrier.dstAccessMask = VK_ACCESS_SHADER_READ_BIT;

        src_stage = VK_PIPELINE_STAGE_TOP_OF_PIPE_BIT;
        dst_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_TRANSFER_DST_OPTIMAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else if (old_layout == VK_IMAGE_LAYOUT_SHADER_READ_ONLY_OPTIMAL && new_layout == VK_IMAGE_LAYOUT_GENERAL) {
        barrier.srcAccessMask = VK_ACCESS_SHADER_READ_BIT;
        barrier.dstAccessMask = VK_ACCESS_TRANSFER_WRITE_BIT;

        src_stage = VK_PIPELINE_STAGE_FRAGMENT_SHADER_BIT;
        dst_stage = VK_PIPELINE_STAGE_TRANSFER_BIT;
    }
    else bs_throwBasiliskF(BSXI_INTERNAL | BSX_UNKNOWN_LAYOUT_TRANSITION, "%s -> %s", bs_layoutName(old_layout), bs_layoutName(new_layout));

    vkCmdPipelineBarrier(
        commands,
        src_stage, dst_stage,
        0,
        0, NULL,
        0, NULL,
        1, &barrier);

    if (_bs_scope_.queue->flags & BS_QUEUE_SINGLE_TIMES_BIT) {
        bsi_pushQueue(_bs_scope_.queue);
        bs_throwVulkan(vkQueueWaitIdle(_bs_scope_.queue->queue));
    }
}

void bs_nameImage(bs_Image* image, const char* name) {
    for (int i = 0; i < bs_imageSwapsCount(image); i++) {
        bsi_nameHandleF(image->_[i].vk_image, VK_OBJECT_TYPE_IMAGE, BS_PRINT_COLOR("%s", BS_PRINT_BLUE_BRIGHT), name);
        bsi_nameHandleF(image->_[i].view, VK_OBJECT_TYPE_IMAGE_VIEW, BS_PRINT_COLOR("%s (View)", BS_PRINT_BLUE_BRIGHT), name);
        bsi_nameHandleF(image->_[i].memory, VK_OBJECT_TYPE_DEVICE_MEMORY, BS_PRINT_COLOR("%s (Memory)", BS_PRINT_BLUE_BRIGHT), name);
    }
}

static void bs_prepareImage(bs_U32 source_id, bs_U32 id, bs_Image* image, VkImageUsageFlags usage_flags, VkImageAspectFlags aspect_flags) {
    image->_->aspect_flags = aspect_flags;
    image->_->usage_flags = usage_flags;

    // image
    VkImageCreateInfo image_ci = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_CREATE_INFO,
        .imageType = VK_IMAGE_TYPE_2D,
        .extent = {
            .width = image->dim.x,
            .height = image->dim.y,
            .depth = 1,
        },
        .mipLevels = 1,
        .arrayLayers = image->num_indices == 0 ? 1 : image->num_indices,
        .format = image->format,
        .tiling = VK_IMAGE_TILING_OPTIMAL,
        .usage = usage_flags,
        .sharingMode = VK_SHARING_MODE_EXCLUSIVE,
        .samples = VK_SAMPLE_COUNT_1_BIT,
    };

    bs_throwVulkan(
        vkCreateImage(
            _bs_instance_->device, 
            &image_ci, 
            NULL, 
            &image->_->vk_image));

    VkMemoryRequirements mem_req;
    vkGetImageMemoryRequirements(
        _bs_instance_->device, 
        image->_->vk_image,
        &mem_req);

    VkMemoryAllocateInfo alloc_i = {
        .sType = VK_STRUCTURE_TYPE_MEMORY_ALLOCATE_INFO,
        .allocationSize = mem_req.size,
        .memoryTypeIndex = bs_queryMemoryType(mem_req.memoryTypeBits, VK_MEMORY_PROPERTY_DEVICE_LOCAL_BIT),
    };

    bs_throwVulkan(
        vkAllocateMemory(
            _bs_instance_->device, 
            &alloc_i, 
            NULL, 
            &image->_->memory));

    vkBindImageMemory(
        _bs_instance_->device, 
        image->_->vk_image,
        image->_->memory,
        0);

    // image view
    VkImageViewType view_type;
    // todo 1d, 3d
    if (image->num_indices > 0) {
        view_type = VK_IMAGE_VIEW_TYPE_2D_ARRAY;
    }
    else {
        view_type = VK_IMAGE_VIEW_TYPE_2D;
    }

    VkImageViewCreateInfo image_view_ci = {
        .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
        .image = image->_->vk_image,
        .viewType = view_type,
        .format = image->format,
        .subresourceRange.aspectMask = image->_->aspect_flags,
        .subresourceRange.levelCount = 1,
        .subresourceRange.layerCount = image->num_indices == 0 ? 1 : image->num_indices,
    };

    bs_throwVulkan(
        vkCreateImageView(
            _bs_instance_->device, 
            &image_view_ci,
            NULL, 
            &image->_->view));

    if (id != 0)
        bs_nameImage(image, bs_idName(source_id, id));
}

static bs_Result bs_depthImage(bs_Object* object, bs_ivec2 dim, int num_indices, bs_Format format, bs_U32 flags) {
    bs_Image* image = object->image;

    bs_U32 num_swaps = flags & BS_IMAGE_SWAPS_BIT ? _bs_settings_.frames_in_flight : 1;

    bs_prepareImage(image->head.source_id, image->head.id, image,
        (flags & BS_IMAGE_ATTACHMENT_BIT       ? VK_IMAGE_USAGE_DEPTH_STENCIL_ATTACHMENT_BIT : 0) |
        (flags & BS_IMAGE_INPUT_ATTACHMENT_BIT ? VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT         : 0) |
        (flags & BS_IMAGE_USAGE_TRANSFER_DST_BIT ? VK_IMAGE_USAGE_TRANSFER_DST_BIT : 0) |
        (flags & BS_IMAGE_USAGE_TRANSFER_SRC_BIT ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0) |
        VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_DEPTH_BIT);

    return BS_RESULT_OK;
}

bs_Result bs_image(bs_Object* object, bs_ivec2 dim, int num_indices, bs_Format format, bs_U32 flags) {
    if (!object)
        return BS_RESULT_INVALID_PARAM;

    if (!object->image)
        return BS_RESULT_OK;

    bs_Image* image = object->image;

    if (!image) 
        return BS_RESULT_OK;

    if (object->flags & BS_OBJECT_ALREADY_EXISTS && !(object->flags & BS_OBJECT_FORCE_DESTROY)) 
        return BS_RESULT_OK;

    bs_destroyImage(image);

    image->dim = dim;
    image->flags = flags;
    image->format = format;
    image->num_indices = num_indices;

    if (num_indices > 0)
        image->indices = bs_calloc(num_indices, sizeof(bs_ImageIndex));

    if (bs_isDepthFormat(format)) 
        return bs_depthImage(object, dim, num_indices, format, flags);

    int num_swaps = flags & BS_IMAGE_SWAPS_BIT ? _bs_settings_.frames_in_flight : 1;
    bs_prepareImage(image->head.source_id, image->head.id, image,
        (flags & BS_IMAGE_ATTACHMENT_BIT            ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0) |
        (flags & BS_IMAGE_INPUT_ATTACHMENT_BIT      ? VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT : 0) |
        (flags & BS_IMAGE_USAGE_TRANSFER_DST_BIT    ? VK_IMAGE_USAGE_TRANSFER_DST_BIT : 0) | 
        (flags & BS_IMAGE_USAGE_TRANSFER_SRC_BIT    ? VK_IMAGE_USAGE_TRANSFER_SRC_BIT : 0) |
        (flags & BS_IMAGE_USAGE_STORAGE_BIT         ? VK_IMAGE_USAGE_STORAGE_BIT : 0) |
        VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT);

    return BS_RESULT_OK;
}



  /*==============================================================================
   * PNG
   *============================================================================*/

static inline void bs_throwLodepng(char* path, unsigned error) {
    bs_throwBasiliskF(BSXI_INTERNAL | BSX_GENERAL, "%s\n%s", lodepng_error_text(error), path);
}

unsigned char* bs_encodePng(size_t* out_size, const unsigned char* data, bs_ivec2 size, bs_PngType type) {
    unsigned char* out = NULL;
    int result = 0;

    switch (type) {
    case BS_PNG_RGB: result = lodepng_encode24(&out, out_size, data, size.x, size.y); break;
    case BS_PNG_RGBA: result = lodepng_encode32(&out, out_size, data, size.x, size.y); break;
    default:
        bs_throwBasilisk(BSX_INVALID_TYPE);
    };

    if (result != 0)
        bs_throwBasiliskF(BSXI_INTERNAL | BSX_GENERAL, "%s", lodepng_error_text(result));

    return out;
}

void bs_savePng(char* data, bs_ivec2 dim, bs_PngType type, char* name) {
    int result = 0;

    switch (type) {
    case BS_PNG_GREY: result = lodepng_encode_file(name, data, dim.x, dim.y, LCT_GREY, 8); break;
    case BS_PNG_RGB: result = lodepng_encode_file(name, data, dim.x, dim.y, LCT_RGB, 8); break;
    case BS_PNG_GREY_ALPHA: result = lodepng_encode_file(name, data, dim.x, dim.y, LCT_GREY_ALPHA, 8); break;
    case BS_PNG_RGBA: result = lodepng_encode_file(name, data, dim.x, dim.y, LCT_RGBA, 8); break;
    default:
        bs_throwBasilisk(BSX_INVALID_TYPE);
    };

    if (result != 0)
        bs_throwBasiliskF(BSXI_INTERNAL | BSX_GENERAL, "%s\n%s", name, lodepng_error_text(result));

    bs_infoF("Saved a PNG of size %d x %d to %s\n", dim.x, dim.y, name);
}

void bs_savePngF(char* data, bs_ivec2 dim, bs_PngType type, const char* format, ...) {
    va_list args;
    va_start(args, format);
    bs_String* formatted = bs_stringV(bs_stringBuilder(), format, args);
    va_end(args);

    return bs_savePng(data, dim, type, formatted->value);
}

unsigned char* bs_inspectPng(const char* path, int* width, int* height, size_t* out_size) {
    unsigned char* data = NULL;

    LodePNGState state;
    lodepng_state_init(&state);

    unsigned error;
    error = lodepng_load_file(&data, out_size, path);
    if (error) {
        bs_throwLodepng(path, error);
        return NULL;
    }

    error = lodepng_inspect(width, height, &state, data, *out_size);
    if (error) {
        bs_throwLodepng(path, error);
        free(data);
        return NULL;
    }

    return data;
}

unsigned char* bs_loadPngMemory(char* data, size_t size, int* width, int* height, int channels_count) {
    int temp_w, temp_h;
    if (!width) width = &temp_w;
    if (!height) height = &temp_h;

    unsigned char* image_data = NULL;
    int error = 0;
    switch (channels_count) {
    case 3: error = lodepng_decode24(&image_data, width, height, data, size); break;
    case 4: error = lodepng_decode32(&image_data, width, height, data, size); break;
    default: bs_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_TYPE, ""); // todo
    }

    if (error) {
        bs_throwLodepng(NULL, error);
        return NULL;
    }

    return image_data;
}

unsigned char* bs_loadPng(const char* path, int* width, int* height, int channels_count) {
    unsigned char* image_data = NULL;
    int error = 0;
    switch (channels_count) {
    case 3: error = lodepng_decode24_file(&image_data, width, height, path); break;
    case 4: error = lodepng_decode32_file(&image_data, width, height, path); break;
    default: bs_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_TYPE, "%s", path); // todo
    }

    if (error) {
        bs_throwLodepng(path, error);
        return NULL;
    }

    return image_data;
}



  /*==============================================================================
   * Image Creation
   *============================================================================*/

bs_Result bs_bitmapImage(bs_Object* object, unsigned char* image_data, bs_ivec2 dim, bs_Format format, bs_ImageBits flags) {
    bs_Result result;

    if (!object)
        return BS_RESULT_INVALID_PARAM;

    if (!object->image)
        return BS_RESULT_OK;

    if (object->flags & BS_OBJECT_ALREADY_EXISTS && !(object->flags & BS_OBJECT_FORCE_DESTROY)) 
        return BS_RESULT_OK;

    bs_Image* image = object->image;
    bs_destroyImage(image);

    bs_U32 swaps_count = flags & BS_IMAGE_SWAPS_BIT ? _bs_settings_.frames_in_flight : 1;

    image->flags = flags;
    image->format = format;
    image->dim = dim;

    int channels_count = bs_hasAlpha(format) ? 4 : 3;
    bs_U32 size = image->dim.x * image->dim.y * channels_count;

   /**
    Staging buffer
    */
    bs_Buffer* staging_buffer = BS_BUFFER(-1, 0, 0)->buffer;
    result = bs_buffer(staging_buffer, size,
        VK_BUFFER_USAGE_TRANSFER_SRC_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0);

    if (result != BS_RESULT_OK) {
        bs_destroyImage(image);
        return result;
    }

    bs_mapBuffer(staging_buffer, size);
    bs_stageImage(staging_buffer, format, image->dim, image_data);
    bs_unmapBuffer(staging_buffer);

   /**
    Create image
    */
    bs_prepareImage(image->head.source_id, object->image->head.id, object->image,
        (flags & BS_IMAGE_ATTACHMENT_BIT ? VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT : 0) |
        (flags & BS_IMAGE_INPUT_ATTACHMENT_BIT ? VK_IMAGE_USAGE_INPUT_ATTACHMENT_BIT : 0) |
        VK_IMAGE_USAGE_TRANSFER_DST_BIT | 
        VK_IMAGE_USAGE_SAMPLED_BIT,
        VK_IMAGE_ASPECT_COLOR_BIT);

    bs_transition(object->image, 0, BS_LAYOUT_UNDEFINED, BS_LAYOUT_TRANSFER_DST_OPTIMAL);
    bs_copyBufferToImage(staging_buffer, object->image, 0, BS_LAYOUT_TRANSFER_DST_OPTIMAL);
    bs_transition(object->image, 0, BS_LAYOUT_TRANSFER_DST_OPTIMAL, BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

   // bs_destroyBuffer(staging_buffer); // queue this or sum shit idk how to deal with this

    return BS_RESULT_OK;
}

void bs_destroyImage(bs_Image* image) {
    bs_U32 num_swaps = image->flags & BS_IMAGE_SWAPS_BIT ? _bs_settings_.frames_in_flight : 1;
    for (int i = 0; i < num_swaps; i++) {
        vkDestroyImageView(_bs_instance_->device, image->_[i].view, NULL);
        vkDestroyImage(_bs_instance_->device, image->_[i].vk_image, NULL);
        vkFreeMemory(_bs_instance_->device, image->_[i].memory, NULL);
        image->_[i].view = image->_[i].vk_image = image->_[i].memory = 0;
    }

    // TODO: make generic
    int id = image->head.id;
    int source_id = image->head.source_id;
    memset(image, 0, sizeof(bs_Image));
    image->head.source_id = source_id;
    image->head.id = id;
}

void bs_resizeImage(bs_Image* image, bs_ivec2 dim, int num_indices) {
    bs_destroyImage(image);
    image->dim = dim;
    image->num_indices = num_indices;
    bs_prepareImage(image->head.source_id, image->head.id, image, image->_->usage_flags, image->_->aspect_flags); // todo id
}

int bs_queryImageIndexHash(bs_Image* image, bs_U64 name_hash, char* name) {
    for (int i = 0; i < image->num_indices; i++) {
        bs_ImageIndex* index = image->indices + i;
        if (index->name_hash == name_hash)
            return i;
    }

    bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Image \"%s\" index \"%s\"", bs_idName(image->head.source_id, image->head.id), name);
    return -1;
}

int bs_queryImageIndex(bs_Image* image, char* name) {
    return bs_queryImageIndexHash(image, bs_stringHash(name), name);
}



  /*==============================================================================
   * Sampler
   *============================================================================*/

void bs_destroySampler(bs_Sampler* sampler) {
    vkDestroySampler(_bs_instance_->device, sampler->_->vk_sampler, NULL);

    // TODO: make generic
    int id = sampler->head.id;
    int source_id = sampler->head.source_id;
    memset(sampler, 0, sizeof(bs_Sampler));
    sampler->head.id = id;
    sampler->head.source_id = source_id;
}

bs_Object* bs_sampler(bs_Object* object, bs_ImageFilter filter, bs_SamplerBits flags) {
    bs_Sampler* sampler = object->sampler;

    if (!sampler) return NULL;
    if (object->flags & BS_OBJECT_ALREADY_EXISTS && !(object->flags & BS_OBJECT_FORCE_DESTROY)) return NULL;

    bs_destroySampler(sampler);

    sampler->filter = filter;
    sampler->flags = flags;

    VkPhysicalDeviceProperties properties = { 0 };
    vkGetPhysicalDeviceProperties(_bs_instance_->physical_device, &properties);

    VkSamplerCreateInfo sampler_i = {
        .sType = VK_STRUCTURE_TYPE_SAMPLER_CREATE_INFO,
        .magFilter = filter,
        .minFilter = filter,
        .addressModeU = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeV = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        .addressModeW = VK_SAMPLER_ADDRESS_MODE_REPEAT,
        //.anisotropyEnable = VK_TRUE,
        .maxAnisotropy = sampler_i.anisotropyEnable ? properties.limits.maxSamplerAnisotropy : 1.0,
        .borderColor = VK_BORDER_COLOR_INT_OPAQUE_BLACK,
        .unnormalizedCoordinates = VK_FALSE,
        .compareEnable = VK_FALSE,
        .compareOp = VK_COMPARE_OP_ALWAYS,
        .mipmapMode = VK_SAMPLER_MIPMAP_MODE_NEAREST,
        .mipLodBias = 0.0f,
        .minLod = 0.0f,
        .maxLod = 0.0f,
    };

    bs_throwVulkan(vkCreateSampler(_bs_instance_->device, &sampler_i, NULL, &sampler->_->vk_sampler));

    return object;
}

void bs_copyImageToBufferAsync(bs_Image* image, bs_Buffer* buffer, int image_index, bs_ImageLayout layout, bs_U64 buffer_offset, bs_ivec2 offset, bs_ivec2 dim) {
    VkCommandBuffer commands = bsi_fetchCommands();
    int image_swap = (image->flags & BS_IMAGE_SWAPS_BIT) ? _bs_swapchain_->frame : 0;
    int buffer_swap = (buffer->flags & BSI_BUFFER_SWAPS_BIT) ? _bs_swapchain_->frame : 0;

    VkBufferImageCopy copy = {
        .bufferOffset = buffer_offset,
        .imageOffset = {
            .x = offset.x,
            .y = image->dim.y - offset.y - dim.y,
        },
        .imageExtent = {
            .width = dim.x,
            .height = dim.y,
            .depth = 1,
        },
        .imageSubresource = {
            .aspectMask = bs_isDepthFormat(image->format) ? VK_IMAGE_ASPECT_DEPTH_BIT : VK_IMAGE_ASPECT_COLOR_BIT,
            .layerCount = 1,
            .baseArrayLayer = image_index,
        },
    };

    if (copy.imageOffset.y < 0 || (copy.imageExtent.height + copy.imageOffset.y) > image->dim.y ||
        copy.imageOffset.x < 0 || (copy.imageExtent.width + copy.imageOffset.x) > image->dim.x) {
        bs_throwBasilisk(BSXI_INTERNAL | BSX_OUT_OF_BOUNDS);
        return;
    }

    int num_comps = bs_hasAlpha(image->format) ? 4 : 3;
    int image_size = dim.x * dim.y * num_comps;
    int buffer_size = buffer->num_bytes - buffer_offset;
    if (image_size > buffer_size) {
        bs_throwBasiliskF(BSXI_INTERNAL | BSX_OUT_OF_BOUNDS, "image size (%d) > buffer size (%d)", image_size, buffer_size);
        return;
    }

    vkCmdCopyImageToBuffer(commands, image->_[image_swap].vk_image, layout, buffer->_[buffer_swap].vk_buffer, 1, &copy);
    return;
}

void bs_copyBufferToImage(bs_Buffer* buffer, bs_Image* image, int index, bs_ImageLayout layout) {
    VkCommandBuffer commands = bsi_fetchCommands();

    VkBufferImageCopy region = {
        .imageSubresource = {
            .aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .mipLevel = 0,
            .baseArrayLayer = index,
            .layerCount = 1,
        },
        .imageExtent.depth = 1,
        .imageExtent.width = image->dim.x,
        .imageExtent.height = image->dim.y,
    };

    vkCmdCopyBufferToImage(
        commands,
        buffer->flags & BSI_BUFFER_SWAPS_BIT ? buffer->_[_bs_swapchain_->frame].vk_buffer : buffer->_->vk_buffer,
        image->flags & BS_IMAGE_SWAPS_BIT ? image->_[_bs_swapchain_->frame].vk_image : image->_->vk_image,
        layout,
        1,
        &region);

    if (_bs_scope_.queue->flags & BS_QUEUE_SINGLE_TIMES_BIT) {
        bsi_pushQueue(_bs_scope_.queue);
        bs_throwVulkan(vkQueueWaitIdle(_bs_scope_.queue->queue));
    }
}

static VkImageAspectFlags bs_imageAspectFlags(bs_Image* image) {
    if (bs_isDepthFormat(image->format)) {
        if (bs_isStencilFormat(image->format))
            return VK_IMAGE_ASPECT_DEPTH_BIT | VK_IMAGE_ASPECT_STENCIL_BIT;

        return VK_IMAGE_ASPECT_DEPTH_BIT;
    }

    return VK_IMAGE_ASPECT_COLOR_BIT;
}

void bs_blit(bs_BlitOperation operation)  {
    VkCommandBuffer commands = bsi_fetchCommands();

    VkImageBlit region = {
        .srcSubresource = {
            .aspectMask = bs_imageAspectFlags(operation.source),
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .srcOffsets[0] = { 0, 0, 0 },
        .srcOffsets[1] = { operation.source_scale.x, operation.source_scale.y, 1 },

        .dstSubresource = {
            .aspectMask = bs_imageAspectFlags(operation.destination),
            .mipLevel = 0,
            .baseArrayLayer = 0,
            .layerCount = 1,
        },
        .dstOffsets[0] = { 0, 0, 0 },
        .dstOffsets[1] = { operation.destination_scale.x, operation.destination_scale.y, 1 },
    };

    vkCmdBlitImage(
        commands,
        operation.source->flags & BS_IMAGE_SWAPS_BIT ? operation.source->_[_bs_swapchain_->frame].vk_image : operation.source->_->vk_image,
        operation.source_layout,
        operation.destination->flags & BS_IMAGE_SWAPS_BIT ? operation.destination->_[_bs_swapchain_->frame].vk_image : operation.destination->_->vk_image,
        operation.destination_layout,
        1, &region,
        VK_FILTER_NEAREST
    );
}



  /*==============================================================================
   * Image File Format
   *============================================================================*/

bs_Result bs_loadImage(bs_Object* object, int package_id, char* resource_name, bs_ImageBits flags) {
    bs_Result result;

    if (!object)
        return BS_RESULT_INVALID_PARAM;

    if (!object->image)
        return BS_RESULT_OK;

    if (bs_objectType(object) != BS_OBJECT_IMAGE)
        return BS_RESULT_INVALID_TYPE;

    bs_Resource* resource = bs_loadResource(package_id, resource_name, 0);

    if (!resource) 
        return NULL;

    bs_BiffHeader* header = resource->data->value;
    if (header->magic != 0x66666962) {
        bs_destroyResource(resource);
        return BS_RESULT_CORRUPTED;
    }

    if (header->version != 1) {
        bs_destroyResource(resource);
        return BS_RESULT_NOT_SUPPORTED;
    }

    if (header->channels_count != 4) {
        bs_destroyResource(resource);
        return BS_RESULT_NOT_IMPLEMENTED;
    }

    object = bs_image(object, bs_iv2(header->width, header->height), header->images_count, BS_FORMAT_R8G8B8A8_UNORM, flags);
    if (!object)
        return NULL;

    unsigned char* data = resource->data->value + sizeof(bs_BiffHeader);

    bs_Buffer* buffer = BS_BUFFER(-1, 0, 0)->buffer;
    result = bs_buffer(buffer, header->width * header->height * header->channels_count,
        BS_BUFFER_USAGE_TRANSFER_SRC_BIT,
        BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0);

    if (result != BS_RESULT_OK) {

        return result;
    }

    bs_mapBuffer(buffer, BS_U32_MAX);

    for (int i = 0; i < header->images_count; i++) {
        bs_BiffPointer* pointer = data;
        data += sizeof(bs_BiffPointer);
        char* name = data;
        int name_length = strlen(name);
        data += sizeof("\n");

        unsigned char* bmp = resource->data->value + pointer->offset;

        object->image->indices[i].name = strdup(name);
        object->image->indices[i].name_hash = bs_stringHash(name);

        bs_stageImage(buffer, BS_FORMAT_R8G8B8A8_UNORM, object->image->dim, bmp);
        bs_transition(object->image, i, BS_LAYOUT_UNDEFINED, BS_LAYOUT_TRANSFER_DST_OPTIMAL);
        bs_copyBufferToImage(buffer, object->image, i, BS_LAYOUT_TRANSFER_DST_OPTIMAL);
        bs_transition(object->image, i, BS_LAYOUT_TRANSFER_DST_OPTIMAL, BS_LAYOUT_SHADER_READ_ONLY_OPTIMAL);

        data += pointer->name_length;
    }

    bs_nameImage(object->image, resource_name);
   //bs_destroyBuffer(buffer);

    resource->image = object->image;

    return object;
}



  /*==============================================================================
   * Atlas File Format
   *============================================================================*/

static bs_vec4 bs_calculateAtlasCoordinates(bs_Atlas* atlas, int texture_id, int frame) {
    bs_AtlasTexture* mapped = atlas->mapped + texture_id;

    float width = (float)mapped->w / (float)atlas->image->dim.x;
    width /= (float)mapped->split;

    return bs_v4(
        (float)mapped->x / (float)atlas->image->dim.x + width * (float)(frame % mapped->split),
        (float)mapped->y / (float)atlas->image->dim.y,
        width,
        (float)mapped->h / (float)atlas->image->dim.y);
}

bs_vec4 bs_atlasCoordinates(bs_Atlas* atlas, int texture_id, int frame) {
    if (texture_id < 0 || texture_id >= atlas->count)
        bs_throwBasiliskF(BSX_OUT_OF_BOUNDS, "Atlas image %d is outside of bounds", texture_id);

    bs_AtlasTexture* mapped = atlas->mapped + texture_id;
    return mapped->coords;
}

bs_vec4 bs_mirrorUV(bs_vec4 uv) {
    return bs_v4(uv.x + uv.z, uv.y, -uv.z, uv.w);
}

bs_vec4 bs_flipUV(bs_vec4 uv) {
#define EPSILON 0.00001f // scuffed
    return bs_v4(uv.x, uv.y + uv.w + EPSILON, uv.z, -uv.w);
}

bs_vec2 bs_atlasSize(bs_Atlas* atlas, int texture) {
    if (texture < 0 || texture >= atlas->count)
        bs_throwBasilisk(BSX_OUT_OF_BOUNDS);

    return bs_v2(atlas->mapped[texture].w / atlas->mapped[texture].split, atlas->mapped[texture].h);
}

int bs_queryAtlasHash(bs_Atlas* atlas, bs_U64 hash, const char* name) {
    for (int i = 0; i < atlas->count; i++) {
        if (atlas->unmapped[i].name_hash == hash)
            return i;
    }

    bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "%s\n@ %s", name, bs_idName(atlas->head.source_id, atlas->head.id));
    return -1;
}

int bs_queryAtlas(bs_Atlas* atlas, const char* name) {
    return bs_queryAtlasHash(atlas, bs_stringHash(name), name);
}

void bs_destroyAtlas(bs_Atlas* atlas) {
    if (atlas->image)
        bs_destroyImage(atlas->image);

    // TODO: make generic
    int id = atlas->head.id;
    int source_id = atlas->head.source_id;
    memset(atlas, 0, sizeof(bs_Atlas));
    atlas->head.source_id = source_id;
    atlas->head.id = id;

    //if (atlas->buffer)
    //	bs_destroyBuffer(atlas->buffer);
}

void bs_splitAtlasTexture(bs_Atlas* atlas, char* name, int split) {
    int id = bs_queryAtlas(atlas, name);
    atlas->mapped[id].split = split;
    atlas->mapped[id].coords = bs_calculateAtlasCoordinates(atlas, id, 0);
}

bs_Result bs_loadAtlasMemory(bs_Object* object, int package_id, char* resource_name, char* data, bs_U32 flags) {
    bs_Result result;

    if (!object)
        return BS_RESULT_INVALID_PARAM;

    if (!object->atlas) 
        return BS_RESULT_OK;

    if (object->flags & BS_OBJECT_ALREADY_EXISTS && !(object->flags & BS_OBJECT_FORCE_DESTROY)) 
        return BS_RESULT_OK;

    bs_Atlas* atlas = object->atlas;

    bs_Image* old_image = atlas->image;
    bs_Buffer* old_buffer = atlas->buffer;
    unsigned char* old_mapped = atlas->mapped;

    bs_destroyAtlas(object->atlas);

    bs_BatlHeader* header = data;
    if (header->magic != 0x6C746162)
        return BS_RESULT_CORRUPTED;

    if (header->version != 1)
        return BS_RESULT_NOT_SUPPORTED;

    if (header->channels_count != 4)
        return BS_RESULT_NOT_IMPLEMENTED;

    bs_Object* image_object = BS_IMAGE(-1, 0, flags & BS_ATLAS_FORCE_CREATE);

    atlas->image = image_object->image;

    unsigned char* bmp = data + header->binary_offset;
    result = bs_bitmapImage(image_object, bmp, bs_iv2(header->width, header->height), BS_FORMAT_R8G8B8A8_UNORM, 0);
    if (result != BS_RESULT_OK) {
        bs_destroyAtlas(object->atlas);
        return result;
    }

    bs_nameImage(atlas->image, resource_name);

    //atlas->name = strdup(resource_name);
    atlas->count = header->images_count;
    atlas->unmapped = bs_malloc(atlas->count * sizeof(*atlas->unmapped));

    if (!old_buffer) {
        bs_Object* buffer_object = BS_BUFFER(-1, 0, 0);
        atlas->buffer = buffer_object->buffer;
        bs_buffer(atlas->buffer, atlas->count * sizeof(*atlas->mapped),
            BS_BUFFER_USAGE_STORAGE_BUFFER_BIT | BS_BUFFER_USAGE_TRANSFER_DST_BIT | BS_BUFFER_USAGE_TRANSFER_SRC_BIT,
            BS_MEMORY_PROPERTY_HOST_VISIBLE_BIT | BS_MEMORY_PROPERTY_HOST_COHERENT_BIT,
            0);

        atlas->mapped = bs_mapBuffer(atlas->buffer, BS_U32_MAX);
    }
    else {
        atlas->mapped = old_mapped;
        atlas->buffer = old_buffer;
    }


    unsigned char* offset = data + sizeof(bs_BatlHeader);
    for (int i = 0; i < atlas->count; i++) {
        bs_BatlPointer* pointer = offset;
        offset += sizeof(bs_BatlPointer);
        char* name = offset;
        offset += pointer->name_length + sizeof("\n");

        atlas->unmapped[i].name = strdup(name);
        atlas->unmapped[i].name_hash = bs_stringHash(name);
        atlas->unmapped[i].category = pointer->category;
        atlas->mapped[i].x = pointer->x,
        atlas->mapped[i].y = pointer->y,
        atlas->mapped[i].w = pointer->w,
        atlas->mapped[i].h = pointer->h,
        atlas->mapped[i].y = atlas->image->dim.y - atlas->mapped[i].y - atlas->mapped[i].h;
        atlas->mapped[i].split = 1;
        atlas->mapped[i].coords = bs_calculateAtlasCoordinates(atlas, i, 0);
        atlas->mapped[i].flags = pointer->flags;
    }

    return BS_RESULT_OK;
}

bs_Object* bs_loadAtlas(bs_Object* object, int package_id, const char* resource_name, bs_U32 flags) {
    bs_Resource* resource = bs_loadResource(package_id, resource_name, 0);
    if (!resource)
        return NULL;
    object = bs_loadAtlasMemory(object, package_id, resource_name, resource->data->value, flags);
    if (object)
        resource->atlas = object->atlas;
    return object;
}
