#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <stdarg.h>
#include <stdlib.h>
#include <assert.h>
#include <inttypes.h>

#include <basilisk-core.h>
#include <bs_internal.h>
#include <vulkan.h>

struct bs_List bs_pipelines[BS_PIPELINE_TYPE_COUNT] = {0};




  /*==============================================================================
   * Descriptors
   *============================================================================*/

static VkDescriptorSetLayout bs_pushDescriptorLayout(bs_BindSet* bind_set) {
    VkDescriptorSetLayoutBinding* layout_bindings = bs_calloc(bind_set->bindings_count, sizeof(VkDescriptorSetLayoutBinding));

    for(int i = 0; i < bind_set->bindings_count; i++) {
        VkDescriptorSetLayoutBinding* layout_binding = layout_bindings + i;
        bs_Binding* binding = bind_set->bindings + i;

        if (binding->stages == 0)
            bs_throwBasilisk(BSXI_INTERNAL | BSX_NO_SHADER_STAGES);
        
        layout_binding->binding = binding->slot;
        layout_binding->descriptorCount = binding->descriptors_count; // == VK_DESCRIPTOR_TYPE_SAMPLED_IMAGE) ? bs_num_objects.images : 1;
        layout_binding->descriptorType = bs_convertBindType(binding->type);
        layout_binding->stageFlags = binding->stages;
    }

    VkDescriptorSetLayoutCreateInfo layout_i = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_LAYOUT_CREATE_INFO,
        .bindingCount = bind_set->bindings_count,
        .pBindings = layout_bindings,
    };
    
    VkDescriptorSetLayout layout = VK_NULL_HANDLE;
    bs_throwVulkan(vkCreateDescriptorSetLayout(_bs_instance_->device, &layout_i, NULL, &layout));
    _bs_instance_->layouts[bind_set->slot] = layout;

    for (int i = 0; i < BS_PIPELINE_TYPE_COUNT; i++) {
        for (int j = 0; j < bs_pipelines[i].count; j++) {
            bs_Pipeline* pipeline = *(bs_Pipeline**)bs_fetchUnit(bs_pipelines + i, j);
            if (bind_set->slot < pipeline->num_bind_sets)
                pipeline->flags |= BS_PIPELINE_NEEDS_UPDATING;
        }
    }

    bs_free(layout_bindings);
    return layout;
}

static VkDescriptorSet bs_pushDescriptorSet(bs_BindSet* bind_set, VkDescriptorSetLayout layout, VkDescriptorPool pool) {
    VkDescriptorSetAllocateInfo alloc_i = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_SET_ALLOCATE_INFO,
        .descriptorPool = pool,
        .descriptorSetCount = 1,
        .pSetLayouts = &layout,
    };

    VkDescriptorSet set = VK_NULL_HANDLE;
    bs_throwVulkan(vkAllocateDescriptorSets(_bs_instance_->device, &alloc_i, &set));
    _bs_instance_->sets[bind_set->slot] = set;

    bsi_nameHandleF(set, VK_OBJECT_TYPE_DESCRIPTOR_SET, "bind set %d", bind_set->slot);

    return set;
}

static void bs_destroyDescriptors() {

} 

const char* bs_serializeBindType(bs_BindType type) {
    switch (type) {
    case BS_BIND_TYPE_STORAGE_BUFFER: return "STORAGE_BUFFER";
    case BS_BIND_TYPE_UNIFORM_BUFFER: return "UNIFORM_BUFFER";
    case BS_BIND_TYPE_SAMPLED_IMAGE: return "SAMPLED_IMAGE";
    case BS_BIND_TYPE_INPUT_ATTACHMENT: return "INPUT_ATTACHMENT";
    case BS_BIND_TYPE_COMBINED_IMAGE_SAMPLER: return "COMBINED_IMAGE_SAMPLER";
    case BS_BIND_TYPE_PUSH_CONSTANT: return "PUSH_CONSTANT";
    case BS_BIND_TYPE_ACCELERATION_STRUCTURE: return "ACCELERATION_STRUCTURE";
    case BS_BIND_TYPE_STORAGE_IMAGE: return "STORAGE_IMAGE";
    default:
        bs_throwBasiliskF(BSX_FAILED_TO_CONVERT, "(bs_BindType)%d\n", type);
        return NULL;
    }
}

bs_BindType bs_deserializeBindType(const char* string) {
    if (strcmp(string, "STORAGE_BUFFER") == 0) return BS_BIND_TYPE_STORAGE_BUFFER;
    else if (strcmp(string, "UNIFORM_BUFFER") == 0) return BS_BIND_TYPE_UNIFORM_BUFFER;
    else if (strcmp(string, "SAMPLED_IMAGE") == 0) return BS_BIND_TYPE_SAMPLED_IMAGE;
    else if (strcmp(string, "INPUT_ATTACHMENT") == 0) return BS_BIND_TYPE_INPUT_ATTACHMENT;
    else if (strcmp(string, "COMBINED_IMAGE_SAMPLER") == 0) return BS_BIND_TYPE_COMBINED_IMAGE_SAMPLER;
    else if (strcmp(string, "PUSH_CONSTANT") == 0) return BS_BIND_TYPE_PUSH_CONSTANT;
    else if (strcmp(string, "ACCELERATION_STRUCTURE") == 0) return BS_BIND_TYPE_ACCELERATION_STRUCTURE;
    else if (strcmp(string, "STORAGE_IMAGE") == 0) return BS_BIND_TYPE_STORAGE_IMAGE;

    bs_throwBasiliskF(BSX_FAILED_TO_CONVERT, "Bind type %s\n", string);
    return 0;
}

VkDescriptorType bs_convertBindType(bs_BindType type) {
    switch (type) {
    case BS_BIND_TYPE_INLINE_UNIFORM_BLOCK: return VK_DESCRIPTOR_TYPE_INLINE_UNIFORM_BLOCK;
    case BS_BIND_TYPE_ACCELERATION_STRUCTURE: return VK_DESCRIPTOR_TYPE_ACCELERATION_STRUCTURE_KHR;
    case BS_BIND_TYPE_MUTABLE_VALVE: return VK_DESCRIPTOR_TYPE_MUTABLE_VALVE;
    default: 
        return type;
    }
}

void bs_pushDescriptorPools() {
//    if (bs_instance->descriptor_pool_needs_update) {
//        bs_instance->descriptor_pool_needs_update = false;
//    }
//    else return;
    bs_warnF("Creating descriptor pools\n");

    static VkDescriptorPool descriptor_pool = VK_NULL_HANDLE;
    if (descriptor_pool) {
        for (int i = 0; i < BS_MAX_NUM_BIND_SETS; i++) {
            if (_bs_instance_->sets[i])
                vkFreeDescriptorSets(_bs_instance_->device, descriptor_pool, 1, _bs_instance_->sets + i); // @todo reuse
            if (_bs_instance_->layouts[i])
                vkDestroyDescriptorSetLayout(_bs_instance_->device, _bs_instance_->layouts[i], NULL);
        }

        vkDestroyDescriptorPool(_bs_instance_->device, descriptor_pool, NULL);
    }

    VkDescriptorPoolSize pool_sizes[BS_NUM_BIND_TYPES] = { 0 };
    VkDescriptorPoolSize pool_sizes_contiguous[BS_NUM_BIND_TYPES] = { 0 };
    bs_U32 num_pool_sizes = 0;

    for(int i = 0; i < _bs_instance_->bind_sets_count; i++) {
        bs_except(BSX_FAILED_TO_QUERY);
        bs_BindSet* bind_set = _bs_instance_->bind_sets + i;
        bs_except(0);
        if (bind_set->bindings_count == 0) continue;

        for(int j = 0; j < bind_set->bindings_count; j++) {
            bs_Binding* binding = bind_set->bindings + j;
            if (binding->type == BS_BIND_TYPE_PUSH_CONSTANT)
                continue;

            if (binding->type < 0 || binding->type >= BS_NUM_BIND_TYPES)
                bs_throwBasilisk(BSXI_INTERNAL | BSX_INVALID_TYPE);

            pool_sizes[binding->type].descriptorCount++;
            pool_sizes[binding->type].type = bs_convertBindType(binding->type);
        }
    }

    for(int i = 0; i < BS_NUM_BIND_TYPES; i++) {
        if (pool_sizes[i].descriptorCount == 0) continue;
        pool_sizes_contiguous[num_pool_sizes++] = pool_sizes[i];
    }

    VkDescriptorPoolCreateInfo pool_i = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_POOL_CREATE_INFO,
        .flags = VK_DESCRIPTOR_POOL_CREATE_FREE_DESCRIPTOR_SET_BIT,
        .poolSizeCount = num_pool_sizes,
        .pPoolSizes = pool_sizes_contiguous,
        .maxSets = BS_MAX_NUM_BIND_SETS,
    };

    bs_throwVulkan(vkCreateDescriptorPool(_bs_instance_->device, &pool_i, NULL, &descriptor_pool));

    for (int i = 0; i < _bs_instance_->bind_sets_count; i++) {
        bs_BindSet* bind_set = _bs_instance_->bind_sets + i;
        if (bind_set->bindings_count == 0) continue;

        bind_set->layout = bs_pushDescriptorLayout(bind_set);
        bind_set->set    = bs_pushDescriptorSet(bind_set, bind_set->layout, descriptor_pool);
    }
}

void bs_pushDescriptors() {
    for (int i = 0; i < _bs_instance_->bind_sets_count; i++) {
        bs_BindSet* bind_set = _bs_instance_->bind_sets + i;
        if (!bind_set->update_template) continue;
        if (!bind_set->needs_update) continue;
        bind_set->needs_update = false;
        vkUpdateDescriptorSetWithTemplate(_bs_instance_->device, bind_set->set, bind_set->update_template, bind_set->descriptors);
    }
}

static inline bool bs_bindIsImageType(bs_BindType type) {
    return type == BS_BIND_TYPE_SAMPLER || type == BS_BIND_TYPE_COMBINED_IMAGE_SAMPLER || type == BS_BIND_TYPE_SAMPLED_IMAGE || type == BS_BIND_TYPE_STORAGE_IMAGE;
}

static void bs_prepareDescriptorTemplate(bs_BindSet* bind_set) {
    if (!bind_set->layout)
        return;

    VkDescriptorUpdateTemplateEntry entries[BS_MAX_NUM_BINDINGS];
    bs_U32 num_entries = 0;

    if (bind_set->update_template)
        vkDestroyDescriptorUpdateTemplate(_bs_instance_->device, bind_set->update_template, NULL);

    for (bs_U32 i = 0; i < bind_set->bindings_count; i++) {
        bs_Binding* binding = bind_set->bindings + i;
        bs_U32 stride = sizeof(bs_Descriptor);

        if (!binding->in_use) continue;
        if (binding->type == BS_BIND_TYPE_PUSH_CONSTANT) continue;
        
        entries[num_entries++] = (VkDescriptorUpdateTemplateEntry) {
            .dstBinding = binding->slot,
            .descriptorCount = binding->descriptors_count,
            .descriptorType = bs_convertBindType(binding->type),
            .offset = binding->location,
            .stride = stride,
        };
    }

    if (num_entries == 0) 
        return;

    VkDescriptorUpdateTemplateCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_DESCRIPTOR_UPDATE_TEMPLATE_CREATE_INFO,
        .pDescriptorUpdateEntries = entries,
        .descriptorUpdateEntryCount = num_entries,
        .pipelineBindPoint = VK_PIPELINE_BIND_POINT_GRAPHICS,
        .templateType = VK_DESCRIPTOR_UPDATE_TEMPLATE_TYPE_DESCRIPTOR_SET,
        .descriptorSetLayout = bind_set->layout,
    };

    vkCreateDescriptorUpdateTemplate(_bs_instance_->device, &ci, NULL, &bind_set->update_template);
}

void bs_pushBindings() {
    for (bs_U32 i = 0; i < _bs_instance_->bind_sets_count; i++) {
        bs_BindSet* bind_set = _bs_instance_->bind_sets + i;
        if (bind_set->bindings_count == 0) continue;

        bs_prepareDescriptorTemplate(bind_set);
    }
}



  /*==============================================================================
   * Bindings
   *============================================================================*/

static bs_Binding* bs_binding(bs_U32 bind_set_slot, bs_U32 slot, bs_Descriptor* descriptors, int descriptors_count) {
   // bs_pushDescriptorPools();

    bs_BindSet* bind_set = bs_queryBindSet(bind_set_slot);
    bs_except(BSX_FAILED_TO_QUERY);
    bs_Binding* binding = bs_queryBinding(bind_set, slot);
    bs_except(0);

    if (!binding) {
        bs_warnF("set = (%d), binding = (%d) is not used by any shader!\n", bind_set_slot, slot);
        return NULL;
    }

    if (!binding->in_use) {
        if (descriptors_count > binding->descriptors_count) {
            bs_throwBasilisk(BSX_OUT_OF_BOUNDS);
            return NULL;
        }

        if ((bind_set->bound_descriptors_count + descriptors_count) > bind_set->descriptors_count) {
            bs_throwBasilisk(BSX_OUT_OF_BOUNDS);
            return NULL;
        }

        binding->location = bind_set->bound_descriptors_count * sizeof(bs_Descriptor);
        bind_set->bound_descriptors_count += descriptors_count;
       // binding->location = bind_set->descriptors_size;
        binding->in_use = true;
    }
    memcpy(((unsigned char*)bind_set->descriptors) + binding->location, descriptors, descriptors_count * sizeof(bs_Descriptor));
    //else if (binding->object.type != type)
    //    bs_throwBasiliskF(BSXI_INTERNAL | BSX_INVALID_TYPE, "Attempted to bind (bs_ObjectType)%d to previously bound (bs_ObjectType)%d (%d, %d)", type, binding->object.type, bind_set_slot, slot);

  //  memcpy(bind_set->descriptors + binding->location, info, info_size);

    bs_queryBindSet(bind_set_slot)->needs_update = true;

    return binding;
}

// ugglygoogoogaga

void bs_bindImages(bs_U32 bind_set_slot, bs_U32 slot, bs_ImageDescriptor* in_descriptors, int images_count) {
    size_t size = images_count * sizeof(bs_Descriptor);
    bs_Descriptor* descriptors = _alloca(size);

    for (int i = 0; i < images_count; i++) {
        bs_ImageDescriptor* descriptor = in_descriptors + i;

        assert(descriptor->sampler != NULL);
        assert(descriptor->image != NULL);

        bs_assertSourceIsType(descriptor->image->head.source_id, BS_OBJECT_IMAGE);

        descriptors[i] = (bs_Descriptor) {
            .as_image = {
                .vk_image_layout = descriptor->layout,
                .vk_image_view = descriptor->image->_[descriptor->image->flags & BS_IMAGE_SWAPS_BIT ? _bs_swapchain->frame : 0].view,
                .vk_sampler = descriptor->sampler ? descriptor->sampler->_->vk_sampler : VK_NULL_HANDLE,
                .image = descriptor->image,
                .sampler = descriptor->sampler,
            },
            .bind_set = bind_set_slot,
            .bind_point = slot,
        };
    }

    bs_binding(bind_set_slot, slot, descriptors, images_count);
}

void bs_bindImage(bs_U32 bind_set_slot, bs_U32 slot, bs_Image* image, bs_Sampler* sampler, bs_ImageLayout layout) {
    return bs_bindImages(bind_set_slot, slot, &(bs_ImageDescriptor) {
        .sampler = sampler,
        .layout = layout,
        .image = image,
    }, 1);
}

void bs_bindBuffer(bs_U32 bind_set_slot, bs_U32 slot, bs_Buffer* buffer) {
    bs_Binding* binding;
    if (!buffer) {
     //   binding = bs_binding(bind_set_slot, slot, &(VkDescriptorBufferInfo) {
     //       .range = VK_WHOLE_SIZE,
     //   }, sizeof(VkDescriptorBufferInfo), BS_BUFFER);
        return;
    }

    binding = bs_binding(bind_set_slot, slot, &(bs_Descriptor) {
        .as_buffer = {
            .vk_buffer = buffer->_[buffer->flags & BSI_BUFFER_SWAPS_BIT ? _bs_swapchain->frame : 0].vk_buffer,
            .vk_range = buffer->num_bytes,
            .buffer = buffer,
        },
        .bind_set = bind_set_slot,
        .bind_point = slot,
    }, 1);
    
    if (!binding) return;

    /*
    binding->object = (bs_Object) {
        .type = BS_BUFFER,
        .buffer = buffer,
    };
    */

    if (!(buffer->flags & BS_BUFFER_IS_NAMED))
        bs_nameBuffer(buffer, binding->name);

    buffer->flags |= BSI_BUFFER_IS_BOUND;
    buffer->bind_set = bind_set_slot;
    buffer->binding = slot;
}

void bs_bindAccelerationStructure(bs_U32 bind_set_slot, bs_U32 slot, bs_RayTracer* ray_tracer) {
    if (!ray_tracer)
        bs_throwBasilisk(BSXI_INTERNAL | BSX_INVALID_PARAM);

   //bs_Binding* binding = bs_binding(bind_set_slot, slot, &ray_tracer->TLAS, 1);
   //if (!binding) return;
    /*
    binding->object = (bs_Object) {
        .type = BS_RAY_TRACER,
        .ray_tracer = ray_tracer,
    };
    */
}

bs_BindSet* bs_queryBindSet(bs_U32 id) {
    assert(id <= _bs_instance_->max_bind_set);

    int bind_set = _bs_instance_->descriptor_lookup[id].bind_set;
    if (bind_set < 0) {
        bsi_throwBasiliskF(BSX_FAILED_TO_QUERY, "Bind set %d", id);
        return NULL;
    }
 
    assert(_bs_instance_->bind_sets[bind_set].slot == id);
    return _bs_instance_->bind_sets + bind_set;

    for (int i = 0; i < _bs_instance_->bind_sets_count; i++) {
        bs_BindSet* bind_set = _bs_instance_->bind_sets + i;
        if (bind_set->slot == id)
            return bind_set;
    }

    bsi_throwBasiliskF(BSX_FAILED_TO_QUERY, "Bind set %d", id);
    return NULL;
}

bs_Binding* bs_queryBinding(bs_BindSet* bind_set, bs_U32 id) {
    assert(id <= bind_set->max_binding);

    int binding = _bs_instance_->descriptor_lookup[bind_set->slot].bindings[id];
    if (binding < 0) {
        bsi_throwBasiliskF(BSX_FAILED_TO_QUERY, "Binding %d", id);
        return NULL;
    }

    assert(_bs_instance_->bindings[binding].slot == id);
    return _bs_instance_->bindings + binding;

    for (int i = 0; i < bind_set->bindings_count; i++) {
        bs_Binding* binding = bind_set->bindings + i;
        if (binding->slot == id)
            return binding;
    }

    bsi_throwBasiliskF(BSX_FAILED_TO_QUERY, "Binding %d", id);
    return NULL;
}

static int bs_compareBindings(const bs_Binding* a, const bs_Binding* b) {
    if (a->set < b->set) return -1;
    else if (a->set > b->set) return 1;
    return 0;
}

static int bs_compareBindSets(const bs_BindSet* a, const bs_BindSet* b) {
    if (a->slot < b->slot) return -1;
    else if (a->slot > b->slot) return 1;
    return 0;
}


void bs_loadBindings(int package_id, const char* path) {
    bs_Resource* resource = bs_loadResource(package_id, path, 0);
    bs_Json json = bs_json(resource->data->value, resource->data->len);

    int bindings_count = bs_fetchJson(&json, BS_JSON_NUMBER, "bindingsCount").as_number;
    int bind_sets_count = bs_fetchJson(&json, BS_JSON_NUMBER, "bindSetsCount").as_number;
    int descriptors_count = bs_fetchJson(&json, BS_JSON_NUMBER, "descriptorsCount").as_number;

    const size_t bind_sets_size = bind_sets_count * sizeof(bs_BindSet);
    const size_t bindings_size = bindings_count * sizeof(bs_Binding);
    const size_t descriptors_size = descriptors_count * sizeof(bs_Descriptor);

    _bs_instance_->bind_sets = bs_realloc(_bs_instance_->bind_sets, bind_sets_size);
    _bs_instance_->bindings = bs_realloc(_bs_instance_->bindings, bindings_size);
    _bs_instance_->descriptors = bs_realloc(_bs_instance_->descriptors, descriptors_size);
    memset(_bs_instance_->bind_sets, 0, bind_sets_size);
    memset(_bs_instance_->bindings, 0, bindings_size);
    memset(_bs_instance_->descriptors, 0, descriptors_size);
    _bs_instance_->descriptor_pool_needs_update = true;

    int binding_offset = 0, descriptor_offset = 0;
    bs_foreachJson(&json, e) {
        if (e.value.type != BS_JSON_OBJECT)
            continue;

        bs_Json root = bs_jsonRoot(&json, e.value.as_object);

        bs_BindType bind_type = bs_deserializeBindType(bs_fetchJson(&root, BS_JSON_STRING, "type").as_string);
        if (bind_type == BS_BIND_TYPE_PUSH_CONSTANT)
            continue;

        bs_Binding* binding = _bs_instance_->bindings + binding_offset++;
        *binding = (bs_Binding) {
            .descriptors_count = bs_fetchJson(&root, BS_JSON_UNDEFINED, "count").as_number,
            .type = bind_type,
            .slot = bs_fetchJson(&root, BS_JSON_NUMBER, "point").as_number,
            .set = bs_fetchJson(&root, BS_JSON_NUMBER, "set").as_number,
            .name = strdup(e.key),
            .size = bs_fetchJson(&root, BS_JSON_NUMBER, "size").as_number,
        };

        bs_JsonValue stages = bs_fetchJson(&root, BS_JSON_ARRAY, "stages");
        for (int j = 0; j < stages.size; j++)
            binding->stages |= bs_deserializeShaderType(stages.as_array.as_strings[j]);

        bs_free(stages.as_array.as_strings);
    }

    bs_assert(bindings_count == binding_offset);

    qsort(_bs_instance_->bindings, bindings_count, sizeof(bs_Binding), bs_compareBindings);

    int current_set = -1;
    _bs_instance_->max_bind_set = 0;
    for (int i = 0; i < bindings_count; i++) {
        bs_Binding* binding = _bs_instance_->bindings + i;
        bs_BindSet* bind_set = _bs_instance_->bind_sets + binding->set;

        bs_assert(binding->set < bind_sets_count);

        if (binding->set != current_set) {
            current_set = binding->set;

            *bind_set = (bs_BindSet) {
                .slot = current_set,
                .bindings = binding,
                .descriptors = _bs_instance_->descriptors + descriptor_offset,
            };
        }

        _bs_instance_->max_bind_set = BS_MAX(_bs_instance_->max_bind_set, bind_set->slot);
        bind_set->max_binding = BS_MAX(bind_set->max_binding, binding->slot);

        bind_set->bindings_count++;
        bind_set->descriptors_count += binding->descriptors_count;
        descriptor_offset += binding->descriptors_count;
    }
 //   qsort(_bs_instance_->bind_sets, bind_sets_count, sizeof(bs_BindSet), bs_compareBindSets);
    assert(descriptors_count == descriptor_offset);

    _bs_instance_->bind_sets_count = bind_sets_count;
    _bs_instance_->bindings_count = bindings_count;
    _bs_instance_->descriptors_count = descriptors_count;

    bs_destroyJson(&json);
    
   /**
    Lookup table
    */
    int total_max_bindings = 0;
    for (int i = 0; i < bind_sets_count; i++)
        total_max_bindings += _bs_instance_->bind_sets[i].max_binding + 1;

    size_t bind_set_lookup_size = (_bs_instance_->max_bind_set + 1) * sizeof(*_bs_instance_->descriptor_lookup);
    size_t binding_lookup_size = total_max_bindings * sizeof(int);
    size_t lookup_size = bind_set_lookup_size + binding_lookup_size;
    _bs_instance_->descriptor_lookup = bs_realloc(_bs_instance_->descriptor_lookup, lookup_size);
    memset(_bs_instance_->descriptor_lookup, -1, lookup_size);
    unsigned char* binding_lookup_offset = ((unsigned char*)_bs_instance_->descriptor_lookup) + bind_set_lookup_size;

    for (int i = 0; i < bind_sets_count; i++) {
        bs_BindSet* bind_set = _bs_instance_->bind_sets + i;

        _bs_instance_->descriptor_lookup[bind_set->slot].bindings = bs_calloc(bind_set->max_binding + 1, sizeof(int));
        memset(_bs_instance_->descriptor_lookup[bind_set->slot].bindings, -1, (bind_set->max_binding + 1) * sizeof(int));
        _bs_instance_->descriptor_lookup[bind_set->slot].bind_set = i;

        for (int j = 0; j < bind_set->bindings_count; j++) {
            bs_Binding* binding = bind_set->bindings + j;
            _bs_instance_->descriptor_lookup[bind_set->slot].bindings[binding->slot] = (binding - _bs_instance_->bindings);
        }
        binding_lookup_offset += (bind_set->max_binding + 1) * sizeof(int);
    }

    bs_pushDescriptorPools();
}



  /*==============================================================================
   * Shaders
   *============================================================================*/

static void bs_readBuffer(bs_Shader* shader, bs_Json* root, const char* name) {
    bs_JsonValue objects = bs_fetchJson(root, BS_JSON_UNDEFINED, name);
    if (!objects.found) return;
    for (int i = 0; i < objects.size; i++) {
        bs_Json object = bs_jsonRoot(root, objects.as_array.as_objects[i]);

        char* name = bs_fetchJson(&object, BS_JSON_STRING, "name").as_string;
        bs_BindType type = bs_deserializeBindType(bs_fetchJson(&object, BS_JSON_STRING, "type").as_string);
        int set = bs_fetchJson(&object, BS_JSON_NUMBER, "set").as_number;
        int point = bs_fetchJson(&object, BS_JSON_NUMBER, "point").as_number;

        if (type == BS_BIND_TYPE_PUSH_CONSTANT) {
            shader->constant_size = bs_fetchJson(&object, BS_JSON_NUMBER, "size").as_number;
            continue;
        }

        shader->bind_sets |= (1 << set);

       // bs_Binding* binding = bs_queryBinding(bs_queryBindSet(set), point);
       // if (binding) {
       //    // binding->stages |= shader->type;
       //  //   bs_pushDescriptorPools();
       // }
    }

    bs_free(objects.as_array.as_objects);
}

static inline void bs_throwUnknownVulkanFormatOfSize(bs_Format format, bs_U32 size) {
    bs_infoF("%d, %d\n", format, size);
    bs_throwBasiliskF(BSXI_INTERNAL | BSX_UNKNOWN_TYPE_SIZE, "Format: %d\nSize: %d\n", format ,size);
}

static inline VkFormat bs_convertFormatFromBaseType(bs_Format format, bs_U32 size) {
    switch (format) {
    case VK_FORMAT_R8_UNORM: {
        if      (size ==  4) return VK_FORMAT_R8_UNORM;
        else if (size ==  8) return VK_FORMAT_R8G8_UNORM;
        else if (size == 12) return VK_FORMAT_R8G8B8_UNORM;
        else if (size == 16) return VK_FORMAT_R8G8B8A8_UNORM;
        else bs_throwUnknownVulkanFormatOfSize(format, size);
    } break;
    case VK_FORMAT_R32_SFLOAT: {
        if      (size ==  4) return VK_FORMAT_R32_SFLOAT;
        else if (size ==  8) return VK_FORMAT_R32G32_SFLOAT;
        else if (size == 12) return VK_FORMAT_R32G32B32_SFLOAT;
        else if (size == 16) return VK_FORMAT_R32G32B32A32_SFLOAT;
        else bs_throwUnknownVulkanFormatOfSize(format, size);
    } break;
    case VK_FORMAT_R32_SINT: {
        if      (size ==  4) return VK_FORMAT_R32_SINT;
        else if (size ==  8) return VK_FORMAT_R32G32_SINT;
        else if (size == 12) return VK_FORMAT_R32G32B32_SINT;
        else if (size == 16) return VK_FORMAT_R32G32B32A32_SINT;
        else bs_throwUnknownVulkanFormatOfSize(format, size);
    } break;
    case VK_FORMAT_R32_UINT: {
        if      (size ==  4) return VK_FORMAT_R32_UINT;
        else if (size ==  8) return VK_FORMAT_R32G32_UINT;
        else if (size == 12) return VK_FORMAT_R32G32B32_UINT;
        else if (size == 16) return VK_FORMAT_R32G32B32A32_UINT;
        else bs_throwUnknownVulkanFormatOfSize(format, size);
    } break;
    }
    bs_throwBasilisk(BSXI_INTERNAL | BSX_INVALID_TYPE);
    return 0;
}

void bs_configureAttribute(const char* name, bs_Format base_format) {
    bs_pushBack(&_bs_config.attributes, &(bs_AttributeType) {
        .base_format = base_format,
        .name = name,
        .name_hash = bs_stringHash(name),
    });
}

static inline void bs_readAttributeType(bs_Attribute* result) {
    for (int i = 0; i < _bs_config.attributes.count; i++) {
        bs_AttributeType* config = bs_fetchUnit(&_bs_config.attributes, i);
        if (config->name_hash == result->name_hash) {
            result->format = bs_convertFormatFromBaseType(config->base_format, result->size);
            return;
        }
    }

    bs_throwBasilisk(BSXI_INTERNAL | BSX_INVALID_VERTEX_ATTRIBUTE);
}

static void bs_compareAttributes(const bs_Attribute* a, const bs_Attribute* b) {
    if      (a->location == b->location) return 0;
    else if (a->location <  b->location) return -1;
    else return 1;
}

static inline void bs_readAttributes(bs_Shader* shader, bs_Json* root) {
    bs_JsonValue objects = bs_fetchJson(root, BS_JSON_ARRAY, "attributes");
    shader->attributes = bs_calloc(objects.size, sizeof(bs_Attribute));
    shader->num_attributes = objects.size;

    for (int i = 0; i < objects.size; i++) {
        bs_Json attribute = bs_jsonRoot(root, objects.as_array.as_objects[i]);
        bs_Attribute* result = shader->attributes + i;

        result->name      = strdup(bs_fetchJson(&attribute, BS_JSON_STRING, "name").as_string);
        result->location  = bs_fetchJson(&attribute, BS_JSON_NUMBER, "location").as_number;
        result->size      = bs_fetchJson(&attribute, BS_JSON_NUMBER, "size").as_number;
        result->name_hash = bs_stringHash(result->name);
        bs_readAttributeType(result);
    }

    qsort(shader->attributes, shader->num_attributes, sizeof(bs_Attribute), bs_compareAttributes);

    for (int i = 0, offset = 0; i < objects.size; i++) {
        bs_Attribute* result = shader->attributes + i;
        result->offset = offset;
        offset += result->size;
    }
}

bs_ShaderType bs_deserializeShaderType(const char* type_name) {
    if (strcmp(type_name, "VERTEX") == 0)
        return BS_VERTEX_SHADER;
    else if (strcmp(type_name, "GEOMETRY") == 0)
        return BS_GEOMETRY_SHADER;
    else if (strcmp(type_name, "FRAGMENT") == 0)
        return BS_FRAGMENT_SHADER;
    else if (strcmp(type_name, "COMPUTE") == 0)
        return BS_COMPUTE_SHADER;
    else if (strcmp(type_name, "RAY_GEN") == 0)
        return BS_RAY_GEN_SHADER;
    else if (strcmp(type_name, "ANY_HIT") == 0)
        return BS_ANY_HIT_SHADER;
    else if (strcmp(type_name, "CLOSEST_HIT") == 0)
        return BS_CLOSEST_HIT_SHADER;
    else if (strcmp(type_name, "MISS") == 0)
        return BS_MISS_SHADER;
    else if (strcmp(type_name, "INTERSECTION") == 0)
        return BS_INTERSECTION_SHADER;

    bs_throwBasiliskF(BSX_FAILED_TO_CONVERT, "Shader type \"%s\"", type_name);
    return 0;
}

const char* bs_serializeShaderType(bs_ShaderType type) {
    switch (type) {
    case BS_VERTEX_SHADER: return "VERTEX";
    case BS_GEOMETRY_SHADER: return "GEOMETRY";
    case BS_FRAGMENT_SHADER: return "FRAGMENT";
    case BS_COMPUTE_SHADER: return "COMPUTE";
    case BS_RAY_GEN_SHADER: return "RAY_GEN";
    case BS_ANY_HIT_SHADER: return "ANY_HIT";
    case BS_CLOSEST_HIT_SHADER: return "CLOSEST_HIT";
    case BS_MISS_SHADER: return "MISS";
    case BS_INTERSECTION_SHADER: return "INTERSECTION";
    }

    bs_throwBasiliskF(BSX_FAILED_TO_CONVERT, "Shader type \"%d\"", type);
    return 0;
}

bs_Result bs_shader(int package_id, const char* name, bs_U32 flags, bs_Resource** out) {
    bs_Resource* resource = bs_loadResource(package_id, name, flags);

    bs_Json metadata = bs_json(resource->data->value, resource->data->len);

    char* base64 = bs_fetchJson(&metadata, BS_JSON_STRING, "SPIR-V").as_string;
    char* type = bs_fetchJson(&metadata, BS_JSON_STRING, "type").as_string;

    bs_U64 code_len = 0;
    bs_U32* spirv;
    if (!bs_decodeBase64(base64, strlen(base64), &code_len, &spirv)) {
        if (out) *out = NULL;
        bs_destroyJson(&metadata);
        return BS_RESULT_OK;
    }

    bs_Shader shader = {
        .type = bs_deserializeShaderType(type),
       // .name = strdup(path),
        .spirv = spirv,
        .spirv_length = code_len,
        .resource = resource,
    };

    bs_readBuffer(&shader, &metadata, "$.uniformBuffers");
    bs_readBuffer(&shader, &metadata, "$.storageBuffers");
    bs_readBuffer(&shader, &metadata, "$.images");
    bs_readBuffer(&shader, &metadata, "$.samplers");
    bs_readBuffer(&shader, &metadata, "$.sampledImages");
    bs_readBuffer(&shader, &metadata, "$.subpasses");
    bs_readBuffer(&shader, &metadata, "$.pushConstants");
    bs_readBuffer(&shader, &metadata, "$.accelerationStructures");
    bs_readBuffer(&shader, &metadata, "$.storageImages");

    if (shader.type == BS_VERTEX_SHADER)
        bs_readAttributes(&shader, &metadata);

    VkShaderModuleCreateInfo shader_ci = {
        .sType = VK_STRUCTURE_TYPE_SHADER_MODULE_CREATE_INFO,
        .codeSize = code_len,
        .pCode = spirv,
    };

    bs_throwVulkan(vkCreateShaderModule(_bs_instance_->device, &shader_ci, NULL, &shader.module));

    bs_destroyJson(&metadata);
    if (!(flags & BS_SHADER_KEEP_SPIRV)) {
        shader.spirv = bs_free(spirv);
        shader.spirv_length = 0;
    }

    bsi_nameHandle(shader.module, VK_OBJECT_TYPE_SHADER_MODULE, name);

    switch (shader.type) {
    case BS_VERTEX_SHADER:
    case BS_FRAGMENT_SHADER:
    case BS_GEOMETRY_SHADER:
        shader.pipeline_type = BS_PIPELINE_GRAPHICS;
        break;
    case BS_COMPUTE_SHADER:
        shader.pipeline_type = BS_PIPELINE_COMPUTE;
        break;
    case BS_RAY_GEN_SHADER:
    case BS_ANY_HIT_SHADER:
    case BS_CLOSEST_HIT_SHADER:
    case BS_MISS_SHADER:
    case BS_INTERSECTION_SHADER:
        shader.pipeline_type = BS_PIPELINE_RAY_TRACE;
        break;
    }

    for (int i = 0; i < bs_pipelines[shader.pipeline_type].count; i++) {
        bs_Pipeline* pipeline = *(bs_Pipeline**)bs_fetchUnit(bs_pipelines + shader.pipeline_type, i);

        for (int j = 0; j < pipeline->shaders_count; j++) {
            if (pipeline->_[j].shader == resource->shader) {
                pipeline->flags |= BS_PIPELINE_NEEDS_UPDATING;
                break;
            }
        }
    }
    //bs_destroyShader(existing);
    if (!resource->shader)
        resource->shader = bs_malloc(sizeof(bs_Shader)); // todo dont do this
    memcpy(resource->shader, &shader, sizeof(bs_Shader));

    if (out)
*out = resource;
    return BS_RESULT_OK;
}

void bs_destroyShader(bs_Shader* shader) {
   // free(shader->name);
    for (int i = 0; i < shader->num_attributes; i++)
        free(shader->attributes[i].name);

    vkDestroyShaderModule(_bs_instance_->device, shader->module, NULL);
}



  /*==============================================================================
   * Pipelines
   *============================================================================*/

static inline bs_Result bs_queryPipeline(bs_PipelineType type, bs_U64 hash, bs_Pipeline** out) {
    for (int i = 0; i < bs_pipelines[type].count; i++) {
        bs_Pipeline* pipeline = *(bs_Pipeline**)bs_fetchUnit(bs_pipelines + type, i);

        if (pipeline->hash == hash) {
            *out = pipeline;
            return BS_RESULT_OK;
        }
    }

    *out = NULL;
    return BS_RESULT_FAILED_TO_QUERY;
}

void bs_pushConstant(bs_Pipeline* pipeline, bs_U32 offset, bs_U32 size, void* data) {
    VkCommandBuffer command_buffer = bsi_fetchCommands();
    if ((offset + size) > pipeline->constant_size)
        return bs_throwBasiliskF(BSXI_INTERNAL | BSX_OUT_OF_BOUNDS, "Pipeline " BS_PRINT_COLOR("%" PRIx64, BS_PRINT_BLUE_BRIGHT) "\nPush Constant\n%d + %d > %d", pipeline->hash, offset, size, pipeline->constant_size);
    vkCmdPushConstants(command_buffer, pipeline->layout, pipeline->shader_stages, offset, size, data);
    if (_bs_scope_.queue->flags & BS_QUEUE_SINGLE_TIMES_BIT) {
        bsi_pushQueue(_bs_scope_.queue);
        bs_throwVulkan(vkQueueWaitIdle(_bs_scope_.queue->queue));
    }
}

static bs_Result bs_createPipelineLayout(bs_Pipeline* pipeline, VkPipelineLayout* out) {
    for (int i = _bs_instance_->bind_sets_count - 1; i >= 0; i--) { // TODO: active bind sets only
        if (!(pipeline->bind_sets & (1 << _bs_instance_->bind_sets[i].slot))) 
            continue;

        pipeline->num_bind_sets = _bs_instance_->bind_sets[i].slot + 1;
        break;
    }

    for (int i = 0; i < pipeline->num_bind_sets; i++) {
        if (_bs_instance_->layouts[i] == NULL) {
            *out = NULL;
            //bs_throwBasiliskF(BSXI_INTERNAL | BSX_GENERAL, "Pipeline cannot be created with unordered bind sets (%d is not used by any shader)", i);
            return BS_RESULT_UNORDERED_BIND_SETS;
        }
    }

    VkPushConstantRange range = { 0 };
    for (int i = 0; i < pipeline->shaders_count; i++) {
        bs_Shader* shader = pipeline->_[i].shader;

        if (!shader) 
            continue;

        if (shader->constant_size == 0) 
            continue;

        range.stageFlags = pipeline->shader_stages |= shader->type;
        range.size = pipeline->constant_size = BS_MAX(range.size, shader->constant_size);
    }

    VkPipelineLayoutCreateInfo pipeline_layout_i = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_LAYOUT_CREATE_INFO,
        .setLayoutCount = pipeline->num_bind_sets,
        .pSetLayouts = _bs_instance_->layouts,
        .pPushConstantRanges = &range,
        .pushConstantRangeCount = range.size > 0 ? 1 : 0,
    };

    bs_throwVulkan(vkCreatePipelineLayout(_bs_instance_->device, &pipeline_layout_i, NULL, &pipeline->layout));

    *out = pipeline->layout;

    return BS_RESULT_OK;
}

static inline VkPipelineShaderStageCreateInfo bs_shaderStage(bs_Shader* shader, bs_SlotBits* bind_sets, VkShaderStageFlags flags) {
    *bind_sets |= shader->bind_sets;

    return (VkPipelineShaderStageCreateInfo) {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_SHADER_STAGE_CREATE_INFO,
        .stage = flags,
        .module = shader->module,
        .pName = "main"
    };
}

static inline bool bs_samePipeline(bs_Pipeline* pipeline, bs_SlotBits bind_sets) {
    return pipeline->bind_sets == bind_sets; // todo maybe need more checks
}

static inline bool bs_pipelineNeedsUpdating(bs_Pipeline* pipeline) {
    return pipeline->flags & BS_PIPELINE_NEEDS_UPDATING;
}

void bs_destroyComputePipeline(bs_Pipeline* pipeline) {
    vkDestroyPipelineLayout(_bs_instance_->device, pipeline->layout, NULL);
    vkDestroyPipeline(_bs_instance_->device, pipeline->pipeline, NULL);
}

typedef struct {
    bs_Shader* compute_shader;
    bs_PipelineFlags flags;
} bs_ComputePipelineHash;

bs_Pipeline* bs_computePipeline(bs_Shader* compute_shader, bs_PipelineFlags flags) {
    if (bs_pipelines[BS_PIPELINE_COMPUTE].capacity == 0)
        bs_pipelines[BS_PIPELINE_COMPUTE] = bs_list(sizeof(bs_Pipeline*), 64);

    bs_U64 hash = bs_hash(&((bs_ComputePipelineHash) { 
        .compute_shader = compute_shader, 
        .flags = flags 
    }), sizeof(bs_ComputePipelineHash));

    bs_except(BSX_FAILED_TO_QUERY);
    bs_Pipeline* existing = bs_queryPipeline(BS_PIPELINE_COMPUTE, hash);
    bs_except(0);

    bool recreated = false;
    if (existing) {
        if (bs_pipelineNeedsUpdating(existing)) {
            bs_destroyPipeline(existing);
            existing->flags &= ~BS_PIPELINE_NEEDS_UPDATING;
            recreated = true;
        }
        else
            return existing;
    }

    bs_Pipeline pipeline = {
        .type = BS_PIPELINE_COMPUTE,
        .shaders_count = 1,
        .bind_sets = compute_shader->bind_sets,
        .hash = hash,
    };

    // TODO this can be improved with new object system
    if (!existing)
        existing = bs_object(-1, 0, sizeof(bs_Pipeline), BS_SWAP_SIZE(bs_Pipeline), pipeline.shaders_count, 0)->pipeline;
    memcpy(existing, &pipeline, sizeof(bs_Pipeline));

    existing->_[0].shader = compute_shader;

    bs_pushBack(bs_pipelines + BS_PIPELINE_COMPUTE, &existing);
    VkComputePipelineCreateInfo pipeline_ci = {
        .sType = VK_STRUCTURE_TYPE_COMPUTE_PIPELINE_CREATE_INFO,
        .stage = bs_shaderStage(compute_shader, &existing->bind_sets, VK_SHADER_STAGE_COMPUTE_BIT),
        .layout = bs_createPipelineLayout(existing),
    };

    bs_throwVulkan(vkCreateComputePipelines(_bs_instance_->device, VK_NULL_HANDLE, 1, &pipeline_ci, NULL, &existing->pipeline));

    const char* blue = (_bs_args_.color_log ? BS_PRINT_BLUE_BRIGHT : "");
    const char* cyan = (_bs_args_.color_log ? BS_PRINT_CYAN : "");
    const char* reset = (_bs_args_.color_log ? BS_PRINT_RESET : "");

    bs_infoF("%s%" PRIx64 "%s (%d) %s%s%s %s\n",
        blue, pipeline.hash, reset,

        compute_shader->head.id,
        (compute_shader->head.id == 0 ? "" : cyan),
        (compute_shader->head.id == 0 ? "" : bs_idName(compute_shader->head.source_id, compute_shader->head.id)),
        reset,

        (recreated ? " (recreated)" : ""));

    return existing;
}

static inline VkStencilOpState bs_mapStencilOpState(bs_StencilOperation op) {
    return (VkStencilOpState) {
        .failOp = op.fail_op,
        .passOp = op.pass_op,
        .depthFailOp = op.depth_fail_op,
        .compareOp = op.compare_op,
        .compareMask = op.compare_mask,
        .writeMask = op.write_mask,
        .reference = op.reference,
    };
}

static inline int bs_defaultEnum(int value, int def) {
    return (value == 0) ? def : (value == -1 ? 0 : value);
}

void bs_destroyPipeline(bs_Pipeline* pipeline) {
    pipeline->name = bs_free(pipeline->name);
    vkDestroyPipelineLayout(_bs_instance_->device, pipeline->layout, NULL);
    vkDestroyPipeline(_bs_instance_->device, pipeline->pipeline, NULL);
}

bs_U64 bs_pipelineHash(bs_PipelineHash* descriptor) {
    bs_U64 hash = 0xcbf29ce484222325;

    for (int i = 0; i < sizeof(bs_PipelineHash); i++) {
        hash ^= ((unsigned char*)descriptor)[i];
        hash *= 0x100000001b3;
    }

    return hash;
}

static void bs_defaultDescriptor(bs_PipelineHash* descriptor) {
    descriptor->depth_comparison = bs_defaultEnum(descriptor->depth_comparison, VK_COMPARE_OP_LESS);
    descriptor->cull_type = bs_defaultEnum(descriptor->cull_type, VK_CULL_MODE_BACK_BIT);
    descriptor->cull_type = (descriptor->cull_type == BS_CULL_MODE_NONE) ? 0 : descriptor->cull_type;
}

bs_Pipeline* bs_pipeline(bs_PipelineHash* descriptor) {
    if (bs_pipelines[BS_PIPELINE_GRAPHICS].capacity == 0)
        bs_pipelines[BS_PIPELINE_GRAPHICS] = bs_list(sizeof(bs_Pipeline*), 64);

    if (!_bs_scope_.renderer) {
        bs_throwBasiliskF(BSX_INVALID_STATE, "Pipelines must be created within a renderer");
        return NULL;
    }

    bool is_dynamic_renderer = bs_rendererIsDynamic(_bs_scope_.renderer);

    descriptor->subpass = _bs_scope_.subpass;
    descriptor->renderer = _bs_scope_.renderer;

    bs_Shader* vs = descriptor->shaders[0];
    bs_Shader* fs = descriptor->shaders[1];
    
    if (vs->type != BS_VERTEX_SHADER ||
        fs->type != BS_FRAGMENT_SHADER) 
        bs_throwBasilisk(BSX_OUT_OF_ORDER);

    bs_Pipeline pipeline = {
        .type = BS_PIPELINE_GRAPHICS,
        .shaders_count = 2,
        .flags = descriptor->flags,
    };

    bs_defaultDescriptor(descriptor);
    pipeline.hash = bs_pipelineHash(descriptor);

    bs_except(BSX_FAILED_TO_QUERY);
    bs_Pipeline* existing = bs_queryPipeline(BS_PIPELINE_GRAPHICS, pipeline.hash);
    bs_except(0);

    bool recreated = false;
    if (existing) {
        if (bs_pipelineNeedsUpdating(existing)) {
            bs_destroyPipeline(existing);
            existing->flags &= ~BS_PIPELINE_NEEDS_UPDATING;
            recreated = true;
        }
        else
            return existing;
    }

    VkGraphicsPipelineCreateInfo pipeline_ci = {
        .sType = VK_STRUCTURE_TYPE_GRAPHICS_PIPELINE_CREATE_INFO,
    };

    VkPipelineRenderingCreateInfo render_info = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_RENDERING_CREATE_INFO,
    };

    VkPipelineDepthStencilStateCreateInfo depth_stencil_state;
    VkPipelineColorBlendAttachmentState blend_states[BS_MAX_NUM_ATTACHMENTS];
    VkFormat format1 = BS_FORMAT_R8G8B8A8_UNORM;

    int num_blend_states = 0;
    if (!is_dynamic_renderer) {
        if (descriptor->subpass >= _bs_scope_.renderer->num_subpasses)
            bs_throwBasiliskF(BSXI_INTERNAL | BSX_OUT_OF_BOUNDS, "Pipeline subpass (%d) falls outside (%d) renderer %s(%s)%s's subpass count (%d)",
                descriptor->subpass,
                _bs_scope_.renderer->head.id,
                (_bs_args_.color_log ? BS_PRINT_CYAN : ""),
                bs_idName(_bs_scope_.renderer->head.source_id, _bs_scope_.renderer->head.id),
                (_bs_args_.color_log ? BS_PRINT_RESET : ""),
                _bs_scope_.renderer->num_subpasses);

        for (int i = 0; i < _bs_scope_.renderer->num_outputs; i++) {
            bs_Output* output = _bs_scope_.renderer->outputs + i;
            if (output->subpass != descriptor->subpass)
                continue;
            if (bs_isDepthFormat(output->image->format))
                continue;

            if (!_bs_features.independent_blend && num_blend_states > 0)
                memcpy(blend_states + num_blend_states++, blend_states, sizeof(VkPipelineColorBlendAttachmentState));
            else
                blend_states[num_blend_states++] = (VkPipelineColorBlendAttachmentState){
                    .colorWriteMask = descriptor->attachments[num_blend_states].skip_write ? 0 : VK_COLOR_COMPONENT_R_BIT | VK_COLOR_COMPONENT_G_BIT | VK_COLOR_COMPONENT_B_BIT | VK_COLOR_COMPONENT_A_BIT,
                    .srcColorBlendFactor = bs_defaultEnum(descriptor->src_color_factor, VK_BLEND_FACTOR_SRC_ALPHA),
                    .dstColorBlendFactor = bs_defaultEnum(descriptor->dst_color_factor, VK_BLEND_FACTOR_ONE_MINUS_SRC_ALPHA),
                    .colorBlendOp = descriptor->color_op,
                    .srcAlphaBlendFactor = bs_defaultEnum(descriptor->src_alpha_factor, VK_BLEND_FACTOR_ONE),
                    .dstAlphaBlendFactor = bs_defaultEnum(descriptor->dst_alpha_factor, VK_BLEND_FACTOR_ZERO),
                    .alphaBlendOp = descriptor->alpha_op,
                    .blendEnable = descriptor->disable_blend ? false : bs_hasAlpha(output->image->format),
                };
        }
    }
    else {
        for (int i = 0; i < _bs_scope_.renderer->num_outputs; i++) {
            bs_Output* output = _bs_scope_.renderer->outputs + i;
            if (bs_isDepthFormat(output->image->format)) {
                render_info.depthAttachmentFormat = output->image->format; // TODO: ensure only one?
            }
        }
    }

    VkPipelineShaderStageCreateInfo shader_stages[] = {
        bs_shaderStage(vs, &pipeline.bind_sets, VK_SHADER_STAGE_VERTEX_BIT),
        bs_shaderStage(fs, &pipeline.bind_sets, VK_SHADER_STAGE_FRAGMENT_BIT)
    };

    // TODO this can be improved with new object system
    if (!existing)
        existing = bs_object(-1, 0, sizeof(bs_Pipeline), BS_SWAP_SIZE(bs_Pipeline), pipeline.shaders_count, 0)->pipeline;
    memcpy(existing, &pipeline, sizeof(bs_Pipeline));

    existing->_[0].shader = vs;
    existing->_[1].shader = fs;

    bs_pushBack(bs_pipelines + BS_PIPELINE_GRAPHICS, &existing);
    bs_createPipelineLayout(existing);

    VkVertexInputAttributeDescription* attributes = NULL;
    VkVertexInputBindingDescription input_binding;
    VkViewport viewport = { 0 };
    VkRect2D scissor = { 0 };
    VkPipelineVertexInputStateCreateInfo vertex_ci;
    VkPipelineInputAssemblyStateCreateInfo assembly_ci;
    VkPipelineRasterizationStateCreateInfo rasterizer_ci;
    VkPipelineMultisampleStateCreateInfo multisampling_ci;
    VkPipelineColorBlendStateCreateInfo color_blending_ci;
    VkPipelineViewportStateCreateInfo viewport_state_ci;

    VkDynamicState states[] = { 
        VK_DYNAMIC_STATE_VIEWPORT,
        VK_DYNAMIC_STATE_SCISSOR,
        VK_DYNAMIC_STATE_LINE_WIDTH
    };

    VkPipelineDynamicStateCreateInfo dynamic_state_i = {
        .sType = VK_STRUCTURE_TYPE_PIPELINE_DYNAMIC_STATE_CREATE_INFO,
        .dynamicStateCount = sizeof(states) / sizeof(VkDynamicState),
        .pDynamicStates = states,
    };
    pipeline_ci.pDynamicState = &dynamic_state_i;

    // add attributes
    attributes = _alloca(vs->num_attributes * sizeof(VkVertexInputAttributeDescription));
    if (vs != NULL) {
        memset(attributes, vs->num_attributes * sizeof(VkVertexInputAttributeDescription), 0);

        int vertex_size = 0;
        for (int i = 0; i < vs->num_attributes; i++)
            vertex_size += vs->attributes[i].size;

        input_binding = (VkVertexInputBindingDescription) {
         //   .binding = object.pipeline->vs_binding,
            .stride = vertex_size,
            .inputRate = VK_VERTEX_INPUT_RATE_VERTEX,
        };

        bs_U32 offset = 0;
        for (int i = 0; i < vs->num_attributes; i++) {
            attributes[i].binding = 0;
            attributes[i].format = vs->attributes[i].format;
            attributes[i].offset = offset;
            attributes[i].location = vs->attributes[i].location;
            offset += vs->attributes[i].size;
        }

        vertex_ci = (VkPipelineVertexInputStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_VERTEX_INPUT_STATE_CREATE_INFO,
            .vertexBindingDescriptionCount = 1,
            .vertexAttributeDescriptionCount = vs->num_attributes,
            .pVertexBindingDescriptions = &input_binding,
            .pVertexAttributeDescriptions = attributes,
        };

        assembly_ci = (VkPipelineInputAssemblyStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_INPUT_ASSEMBLY_STATE_CREATE_INFO,
            .topology = bs_defaultEnum(descriptor->topology_type, VK_PRIMITIVE_TOPOLOGY_TRIANGLE_LIST),
            .primitiveRestartEnable = descriptor->restart_primitive,
        };

        pipeline_ci.pVertexInputState = &vertex_ci;
        pipeline_ci.pInputAssemblyState = &assembly_ci;
    }
    
    bs_ivec2 resolution = bs_resolution();
    viewport.width = resolution.x;
    viewport.height = resolution.y;
    viewport.minDepth = 0.0;
    viewport.maxDepth = 1.0;

    scissor.extent.width = resolution.x;
    scissor.extent.height = resolution.y;

    viewport_state_ci = (VkPipelineViewportStateCreateInfo){
        .sType = VK_STRUCTURE_TYPE_PIPELINE_VIEWPORT_STATE_CREATE_INFO,
        .pViewports = &viewport,
        .pScissors = &scissor,
        .viewportCount = 1,
        .scissorCount = 1,
    };

    pipeline_ci.pViewportState = &viewport_state_ci;

    if (fs) {
        rasterizer_ci = (VkPipelineRasterizationStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_RASTERIZATION_STATE_CREATE_INFO,
            .polygonMode = descriptor->polygon_type,
            .lineWidth = 1.0f,
            .cullMode = descriptor->cull_type,
            .depthClampEnable = descriptor->clamp_depth,
            .frontFace = VK_FRONT_FACE_COUNTER_CLOCKWISE,
        };

        multisampling_ci = (VkPipelineMultisampleStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_MULTISAMPLE_STATE_CREATE_INFO,
            .rasterizationSamples = VK_SAMPLE_COUNT_1_BIT,
            .minSampleShading = 1.0f,
        };

        color_blending_ci = (VkPipelineColorBlendStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_COLOR_BLEND_STATE_CREATE_INFO,
            .logicOp = VK_LOGIC_OP_COPY,
            .attachmentCount = num_blend_states,
            .pAttachments = blend_states
        };

        bool has_stencil =
            descriptor->stencil_front.compare_op != 0 || descriptor->stencil_front.depth_fail_op != 0 ||
            descriptor->stencil_front.fail_op != 0 || descriptor->stencil_front.pass_op != 0 ||
            descriptor->stencil_back.compare_op != 0 || descriptor->stencil_back.depth_fail_op != 0 ||
            descriptor->stencil_back.fail_op != 0 || descriptor->stencil_back.pass_op != 0;

        depth_stencil_state = (VkPipelineDepthStencilStateCreateInfo) {
            .sType = VK_STRUCTURE_TYPE_PIPELINE_DEPTH_STENCIL_STATE_CREATE_INFO,
            .depthTestEnable = !descriptor->skip_depth_test,
            .depthWriteEnable = !descriptor->skip_depth_write,
            .depthCompareOp = descriptor->depth_comparison,
            .depthBoundsTestEnable = VK_FALSE,
            .minDepthBounds = 0.0f,
            .maxDepthBounds = 1.0f,
            .stencilTestEnable = has_stencil && !descriptor->skip_stencil_test,
            .front = bs_mapStencilOpState(descriptor->stencil_front),
            .back = bs_mapStencilOpState(descriptor->stencil_back),
        };

        pipeline_ci.pRasterizationState = &rasterizer_ci;
        pipeline_ci.pMultisampleState = &multisampling_ci;
        pipeline_ci.pColorBlendState = &color_blending_ci;
        if (!is_dynamic_renderer)
            pipeline_ci.pDepthStencilState = BS_RENDERER_SUBPASS_HAS_DEPTH(_bs_scope_.renderer->flags, 0) ? &depth_stencil_state : NULL; // todo should this be 0?
        else
            pipeline_ci.pDepthStencilState = &depth_stencil_state;
    }

    pipeline_ci.stageCount = sizeof(shader_stages) / sizeof(*shader_stages);
    pipeline_ci.pStages = shader_stages;
    pipeline_ci.layout = existing->layout;
    if (!is_dynamic_renderer)
        pipeline_ci.renderPass = _bs_scope_.renderer->render_pass;
    else
        pipeline_ci.pNext = &render_info;
    pipeline_ci.basePipelineIndex = -1;
    pipeline_ci.subpass = descriptor->subpass;

    bs_throwVulkan(vkCreateGraphicsPipelines(_bs_instance_->device, VK_NULL_HANDLE, 1, &pipeline_ci, NULL, &existing->pipeline));

    existing->name = bs_stringF(existing->name, BS_PRINT_COLOR("%" PRIx64, BS_PRINT_BLUE_BRIGHT), pipeline.hash);
    bsi_nameHandle(existing->pipeline, VK_OBJECT_TYPE_PIPELINE, existing->name->value);

    char* vs_name = vs->resource->name;
    char* fs_name = fs->resource->name;
    bs_infoF(BS_PRINT_COLOR("%" PRIx64, BS_PRINT_BLUE_BRIGHT) BS_PRINT_COLOR(" %s", BS_PRINT_CYAN) " -> " BS_PRINT_COLOR("%s", BS_PRINT_CYAN) "%s\n",
        pipeline.hash,
        vs_name,
        fs_name,
        (recreated ? " (recreated)" : ""));

    return existing;
}



  /*==============================================================================
   * Ray Tracing
   *============================================================================*/

static inline VkRayTracingShaderGroupCreateInfoKHR bs_shaderGroup(VkRayTracingShaderGroupTypeKHR type, bs_U32 general_id, bs_U32 closest_hit_id) {
    return (VkRayTracingShaderGroupCreateInfoKHR) {
        .sType = VK_STRUCTURE_TYPE_RAY_TRACING_SHADER_GROUP_CREATE_INFO_KHR,
        .type = type,
        .generalShader = general_id,
        .closestHitShader = closest_hit_id,
        .anyHitShader = VK_SHADER_UNUSED_KHR,
        .intersectionShader = VK_SHADER_UNUSED_KHR,
    };
}

bs_Result bs_rayTracingPipeline(bs_RayTracePipelineHash* pipeline_hash, bs_Pipeline** out) {
    bs_Result result;

    if (pipeline_hash == NULL)
        return BS_RESULT_INVALID_PARAM;

    if (bs_pipelines[BS_PIPELINE_RAY_TRACE].capacity == 0)
        bs_pipelines[BS_PIPELINE_RAY_TRACE] = bs_list(sizeof(bs_Pipeline*), 8);

    bs_U64 hash = bs_hash(pipeline_hash, sizeof(bs_RayTracePipelineHash));

    bs_Pipeline* existing;
    bs_queryPipeline(BS_PIPELINE_RAY_TRACE, hash, &existing);

    if (out)
        *out = existing;

    bool recreated = false;
    if (existing) {
        if (bs_pipelineNeedsUpdating(existing)) {
            bs_destroyPipeline(existing);
            existing->flags &= ~BS_PIPELINE_NEEDS_UPDATING;
            recreated = true;
        }
        else
            return BS_RESULT_OK;
    }

    bs_U32 bind_sets = 0;
    for (int i = 0; i < pipeline_hash->ray_tracer->groups_count; i++)
        bind_sets |= pipeline_hash->ray_tracer->_[i].shader->bind_sets;
    
    bs_Pipeline pipeline = {
        .type = BS_PIPELINE_RAY_TRACE,
        .shaders_count = 5,
        .bind_sets = bind_sets,
        .hash = hash,
    };

    if (!existing)
        existing = bs_object(-1, 0, sizeof(bs_Pipeline), BS_SWAP_SIZE(bs_Pipeline), pipeline.shaders_count, 0)->pipeline;
    memcpy(existing, &pipeline, sizeof(bs_Pipeline));

   // existing->_[0].shader = pipeline_hash->ray_gen_shader;
   // existing->_[1].shader = pipeline_hash->closest_hit_shader;
   // existing->_[2].shader = pipeline_hash->miss_shader;
   // existing->_[3].shader = pipeline_hash->intersection_shader;
   // existing->_[4].shader = pipeline_hash->any_hit_shader;

    bs_pushBack(bs_pipelines + BS_PIPELINE_RAY_TRACE, &existing);

    bs_createPipelineLayout(existing);

    VkPipelineShaderStageCreateInfo stages[5] = { 0 };
    VkRayTracingShaderGroupCreateInfoKHR shader_groups[5] = { 0 };
    int count = pipeline_hash->ray_tracer->groups_count;
    for (int i = 0; i < count; i++) {
        shader_groups[i] = bs_shaderGroup(VK_RAY_TRACING_SHADER_GROUP_TYPE_GENERAL_KHR, i, VK_SHADER_UNUSED_KHR);
        stages[i] = bs_shaderStage(pipeline_hash->ray_tracer->_[i].shader, &pipeline.bind_sets, pipeline_hash->ray_tracer->_[i].shader->type);
    }

    VkRayTracingPipelineCreateInfoKHR ci = {
        .sType = VK_STRUCTURE_TYPE_RAY_TRACING_PIPELINE_CREATE_INFO_KHR,
        .stageCount = count,
        .pStages = stages,
        .groupCount = count,
        .pGroups = shader_groups,
        .maxPipelineRayRecursionDepth = 1,
        .layout = existing->layout,
    };

    result = bs_convertVulkanResult(_bs_procs_.vkCreateRayTracingPipelinesKHR(
        _bs_instance_->device, 
        NULL, 
        NULL, 
        1, 
        &ci, 
        NULL, 
        &existing->pipeline)
    );

    if (result != BS_RESULT_OK) {
        bs_destroyPipeline(existing);
        return result;
    }

    const char* blue = (_bs_args_.color_log ? BS_PRINT_BLUE_BRIGHT : "");
    const char* cyan = (_bs_args_.color_log ? BS_PRINT_CYAN : "");
    const char* reset = (_bs_args_.color_log ? BS_PRINT_RESET : "");

    bs_infoF("%s%" PRIx64 "%s\n", blue, pipeline.hash, reset);

    /**
     Binding table
     */
    bs_RayTracer* tracer = pipeline_hash->ray_tracer;

    tracer->record_size = bs_alignUp(_bs_props.shader_group_handle_size, _bs_props.shader_group_base_alignment);
    bs_U32 buffer_size = tracer->groups_count * tracer->record_size;

    char* shader_handle_storage = bs_malloc(buffer_size);
    _bs_procs_.vkGetRayTracingShaderGroupHandlesKHR(_bs_instance_->device, existing->pipeline, 0, tracer->groups_count, buffer_size, shader_handle_storage);

    existing->binding_table = BS_BUFFER(-1, 0, 0);
    bs_buffer(existing->binding_table, buffer_size,
        VK_BUFFER_USAGE_SHADER_BINDING_TABLE_BIT_KHR | VK_BUFFER_USAGE_SHADER_DEVICE_ADDRESS_BIT,
        VK_MEMORY_PROPERTY_HOST_VISIBLE_BIT | VK_MEMORY_PROPERTY_HOST_COHERENT_BIT,
        0)->buffer;
    char* binding_table_data = bs_mapBuffer(existing->binding_table, BS_U32_MAX);

    for (int i = 0; i < tracer->groups_count; i++) {
        memcpy(binding_table_data, shader_handle_storage + i * _bs_props.shader_group_handle_size, _bs_props.shader_group_handle_size);
        binding_table_data += tracer->record_size;
    }

  // bs_free(shader_handle_storage);

    return existing;
}