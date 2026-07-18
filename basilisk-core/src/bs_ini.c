
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

#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan.h>
#include <vulkan/vulkan_core.h>

#include <basilisk-core.h>
#include <bs_internal.h>
#include <bs_prevalidation.gen.h>
#include <bs_validation.gen.h>

const char* validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

bs_Args _bs_args_ = { 0 };
bs_Features _bs_features_ = { 0 };
bs_Props _bs_props_ = { 0 };
bs_Procs _bs_procs_ = { 0 };
bs_Config _bs_config_ = {
    .attributes.unit_size = sizeof(bs_AttributeType),
};

bs_Scope _bs_scope_ = { 0 };
bs_Instance* _bs_instance_ = NULL;
bs_IO _bs_io_ = { 0 };
int _bs_image_index_ = 0;

BSAPI bs_Instance* _bs_instance() { return _bs_instance_; }
BSAPI bs_Args* _bs_args() { return &_bs_args_; }
BSAPI bs_Features* _bs_features() { return &_bs_features_; }
BSAPI bs_Props* _bs_props() { return &_bs_props_; }
BSAPI bs_Config* _bs_config() { return &_bs_config_; }
BSAPI bs_Scope* _bs_scope() { return &_bs_scope_; }
BSAPI bs_IO* _bs_io() { return &_bs_io_; }

BSAPI void _bsi_nameHandle(bs_U64 handle, bs_U32 type, char* name, int name_length) {
    PFN_vkSetDebugUtilsObjectNameEXT pfn_vkSetDebugUtilsObjectNameEXT = 
        (PFN_vkSetDebugUtilsObjectNameEXT)vkGetDeviceProcAddr(_bs_instance_->device, "vkSetDebugUtilsObjectNameEXT");

    const VkDebugUtilsObjectNameInfoEXT name_i = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .pNext = NULL,
        .objectType = type,
        .objectHandle = handle,
        .pObjectName = name,
    };

    pfn_vkSetDebugUtilsObjectNameEXT(_bs_instance_->device, &name_i);
}

static const char* _bs_vulkanObjectName(VkObjectType type) {
    switch (type) {
    case VK_OBJECT_TYPE_INSTANCE: return "VK_OBJECT_TYPE_INSTANCE";
    case VK_OBJECT_TYPE_PHYSICAL_DEVICE: return "VK_OBJECT_TYPE_PHYSICAL_DEVICE";
    case VK_OBJECT_TYPE_DEVICE: return "VK_OBJECT_TYPE_DEVICE";
    case VK_OBJECT_TYPE_QUEUE: return "VK_OBJECT_TYPE_QUEUE";
    case VK_OBJECT_TYPE_SEMAPHORE: return "VK_OBJECT_TYPE_SEMAPHORE";
    case VK_OBJECT_TYPE_COMMAND_BUFFER: return "VK_OBJECT_TYPE_COMMAND_BUFFER";
    case VK_OBJECT_TYPE_FENCE: return "VK_OBJECT_TYPE_FENCE";
    case VK_OBJECT_TYPE_DEVICE_MEMORY: return "VK_OBJECT_TYPE_DEVICE_MEMORY";
    case VK_OBJECT_TYPE_BUFFER: return "VK_OBJECT_TYPE_BUFFER";
    case VK_OBJECT_TYPE_IMAGE: return "VK_OBJECT_TYPE_IMAGE";
    case VK_OBJECT_TYPE_EVENT: return "VK_OBJECT_TYPE_EVENT";
    case VK_OBJECT_TYPE_QUERY_POOL: return "VK_OBJECT_TYPE_QUERY_POOL";
    case VK_OBJECT_TYPE_BUFFER_VIEW: return "VK_OBJECT_TYPE_BUFFER_VIEW";
    case VK_OBJECT_TYPE_IMAGE_VIEW: return "VK_OBJECT_TYPE_IMAGE_VIEW";
    case VK_OBJECT_TYPE_SHADER_MODULE: return "VK_OBJECT_TYPE_SHADER_MODULE";
    case VK_OBJECT_TYPE_PIPELINE_CACHE: return "VK_OBJECT_TYPE_PIPELINE_CACHE";
    case VK_OBJECT_TYPE_PIPELINE_LAYOUT: return "VK_OBJECT_TYPE_PIPELINE_LAYOUT";
    case VK_OBJECT_TYPE_RENDER_PASS: return "VK_OBJECT_TYPE_RENDER_PASS";
    case VK_OBJECT_TYPE_PIPELINE: return "VK_OBJECT_TYPE_PIPELINE";
    case VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT: return "VK_OBJECT_TYPE_DESCRIPTOR_SET_LAYOUT";
    case VK_OBJECT_TYPE_SAMPLER: return "VK_OBJECT_TYPE_SAMPLER";
    case VK_OBJECT_TYPE_DESCRIPTOR_POOL: return "VK_OBJECT_TYPE_DESCRIPTOR_POOL";
    case VK_OBJECT_TYPE_DESCRIPTOR_SET: return "VK_OBJECT_TYPE_DESCRIPTOR_SET";
    case VK_OBJECT_TYPE_FRAMEBUFFER: return "VK_OBJECT_TYPE_FRAMEBUFFER";
    case VK_OBJECT_TYPE_COMMAND_POOL: return "VK_OBJECT_TYPE_COMMAND_POOL";
    }

    return "unknown";
}

static VKAPI_ATTR VkBool32 VKAPI_CALL _bs_debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* param)
{
    static bs_String* message;

    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        message = _bs_stringF(message, "Message name: %s\n", data->pMessageIdName ? data->pMessageIdName : "N/A\n");
        message = _bs_appendStringF(message, "Message: %s\n", data->pMessage);

        if (data->objectCount > 0) {
            for (uint32_t i = 0; i < data->objectCount; i++) {
                const VkDebugUtilsObjectNameInfoEXT* obj = &data->pObjects[i];
                message = _bs_appendStringF(message, "Object %d:\n", i);
                message = _bs_appendStringF(message, "  Handle: 0x%llx\n", (unsigned long long)obj->objectHandle);
                message = _bs_appendStringF(message, "  Type:   %s\n", _bs_vulkanObjectName(obj->objectType));
                if (obj->pObjectName)
                    message = _bs_appendStringF(message, "  Name:   %s\n", obj->pObjectName);
            }
        }

        _bs_warn(message->value, message->len);
    }
    else {
        _bs_warnF("%s\n", data->pMessage);
    }

    // _bs_free(message);

    return VK_FALSE;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL _bs_reportCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT type,
    uint64_t object,
    size_t location,
    int32_t code,
    const char* prefix,
    const char* message,
    void* param) {

 //   _bs_infoF("%s\n", message);
    return VK_FALSE;
}

static bool _bs_checkValidationLayerSupport() {
    bs_U32 num_layers;
    vkEnumerateInstanceLayerProperties(&num_layers, NULL);

    VkLayerProperties* layers = _bs_malloc(num_layers * sizeof(VkLayerProperties));
    vkEnumerateInstanceLayerProperties(&num_layers, layers);

    bool found_all = true;
    int num_validation_layers = sizeof(validation_layers) / sizeof(const char *);
    for(int i = 0; i < num_validation_layers; i++) {
        bool found = false;

        for(int j = 0; j < num_layers; j++) {
            if (strcmp(validation_layers[i], layers[j].layerName) == 0) {
                found = true;
                break;
            }
	    }

        if (!found) {
            _bs_warnF("Vulkan validation layer %s is not supported\n", validation_layers[i]);
            found_all = false;
        }
    }

    _bs_free(layers);
    return found_all;
}

static inline bool _bs_addInstanceExtension(const char** extensions, bs_U32* extensions_count, const char* name, VkExtensionProperties* available_extensions, bs_U32 available_extensions_count) {
    for (bs_U32 i = 0; i < available_extensions_count; i++) {
        if (strcmp(name, available_extensions[i].extensionName) == 0) {
            extensions[(*extensions_count)++] = available_extensions[i].extensionName;
            return true;
        }
    }

    return false;
}

static void _bs_prepareInstance() {
    VkResult result;

    if (_bs_args_.use_validation_layers)
        _bs_args_.use_validation_layers = _bs_checkValidationLayerSupport();

    const char* extensions[16];
    bs_U32 extensions_count = 0;

    bs_U32 available_extensions_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &available_extensions_count, NULL);
    VkExtensionProperties* available_extensions = malloc(available_extensions_count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &available_extensions_count, available_extensions);

#define BS_ADD_INSTANCE_EXTENSION(name) \
    _bs_addInstanceExtension(extensions, &extensions_count, name, available_extensions, available_extensions_count)

#ifdef _WIN32
    if (BS_ADD_INSTANCE_EXTENSION(VK_KHR_WIN32_SURFACE_EXTENSION_NAME))
         _bs_instance_->extensions.surface_type = BS_SURFACE_TYPE_WIN32;
#elif __linux__
    if (BS_ADD_INSTANCE_EXTENSION(VK_KHR_WAYLAND_SURFACE_EXTENSION_NAME))
        _bs_instance_->extensions.surface_type = BS_SURFACE_TYPE_WAYLAND;
    else if (BS_ADD_INSTANCE_EXTENSION(VK_KHR_XLIB_SURFACE_EXTENSION_NAME))
        _bs_instance_->extensions.surface_type = BS_SURFACE_TYPE_X11;
#endif
    else if (BS_ADD_INSTANCE_EXTENSION(VK_EXT_HEADLESS_SURFACE_EXTENSION_NAME)) {
        _bs_warnF("Only off-screen rendering is available\n");
        _bs_instance_->extensions.surface_type = BS_SURFACE_TYPE_HEADLESS;
    }
    else {
        _bs_warnF("No instance surface extension found\n");
    }
    BS_ADD_INSTANCE_EXTENSION(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
    BS_ADD_INSTANCE_EXTENSION(VK_EXT_DEBUG_REPORT_EXTENSION_NAME);
    BS_ADD_INSTANCE_EXTENSION(VK_KHR_SURFACE_EXTENSION_NAME);
    BS_ADD_INSTANCE_EXTENSION(VK_EXT_VALIDATION_FEATURES_EXTENSION_NAME);

    VkApplicationInfo app_i = {
        .sType = VK_STRUCTURE_TYPE_APPLICATION_INFO,
        .pApplicationName = "",
        .applicationVersion = VK_MAKE_API_VERSION(0, 1, 4, 0),
        .pEngineName = "BASILISK",
        .engineVersion = VK_MAKE_API_VERSION(0, 1, 4, 0),
        .apiVersion = VK_API_VERSION_1_4,
    };

    const VkValidationFeatureEnableEXT enabled_features[] = {
        VK_VALIDATION_FEATURE_ENABLE_SYNCHRONIZATION_VALIDATION_EXT,
     //   VK_VALIDATION_FEATURE_ENABLE_DEBUG_PRINTF_EXT,
       // VK_VALIDATION_FEATURE_ENABLE_GPU_ASSISTED_EXT,
    };

    VkValidationFeaturesEXT features = {
        .sType = VK_STRUCTURE_TYPE_VALIDATION_FEATURES_EXT,
        .pEnabledValidationFeatures = enabled_features,
        .enabledValidationFeatureCount = sizeof(enabled_features) / sizeof(VkValidationFeatureEnableEXT),
        .pNext = NULL,
    };

    VkInstanceCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_INSTANCE_CREATE_INFO,
        .pApplicationInfo = &app_i,
        .enabledExtensionCount = extensions_count,
        .ppEnabledExtensionNames = extensions,
        .ppEnabledLayerNames = validation_layers,
        .enabledLayerCount = _bs_args_.use_validation_layers ? sizeof(validation_layers) / sizeof(const char*) : 0,
     //  .pNext = &features,
    };

    result = vkCreateInstance(&ci, NULL, &_bs_instance_->instance);
    if (result != VK_SUCCESS) {
        _bs_criticalF("Failed to create instance (Vulkan result %d)\n", result);
        return;
    }

    free(available_extensions);

    VkDebugUtilsMessengerEXT messenger = { 0 };
    const VkDebugUtilsMessengerCreateInfoEXT debug_ci = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = _bs_debugCallback,
    };

    PFN_vkCreateDebugUtilsMessengerEXT create_messenger = 
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_bs_instance_->instance, "vkCreateDebugUtilsMessengerEXT");

    PFN_vkCreateDebugUtilsMessengerEXT create_reporter = 
        (PFN_vkCreateDebugUtilsMessengerEXT)vkGetInstanceProcAddr(_bs_instance_->instance, "vkCreateDebugReportCallbackEXT");

    if (create_messenger)
        create_messenger(_bs_instance_->instance, &debug_ci, NULL, &messenger);

    VkDebugReportCallbackCreateInfoEXT report_ci = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
        .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT,
        .pfnCallback = _bs_reportCallback,
    };

    VkDebugReportCallbackEXT reporter = { 0 };
    if (create_reporter)
        create_reporter(_bs_instance_->instance, &report_ci, NULL, &reporter);
}

static void _bs_createSurface(bs_Window* window) {
    VkResult result = VK_SUCCESS;

    if (_bs_instance_->extensions.surface_type == BS_SURFACE_TYPE_WIN32) {
        VkWin32SurfaceCreateInfoKHR ci = {
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = GetModuleHandle(0),
            .hwnd = window->hwnd,
        };

        result = vkCreateWin32SurfaceKHR(_bs_instance_->instance, &ci, NULL, &window->surface);
    }
    else if (_bs_instance_->extensions.surface_type == BS_SURFACE_TYPE_HEADLESS) {
        VkHeadlessSurfaceCreateInfoEXT ci = {
            .sType = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT,
        };

        result = vkCreateHeadlessSurfaceEXT(_bs_instance_->instance, &ci, NULL, &window->surface);
    }
    else {
        _bs_warnF("Surface type %d is not supported\n", _bs_instance_->extensions.surface_type);
    }

    if (result != VK_SUCCESS) {
        _bs_warnF("Failed to create surface for window \"%s\" (Vulkan result = %d)\n", window->title, result);
    }
}

static inline void _bs_logPhysicalDeviceInfo(int i, VkPhysicalDevice device) {
    VkPhysicalDeviceProperties properties;
    vkGetPhysicalDeviceProperties(device, &properties);

    const char* type = NULL;

    switch (properties.deviceType) {
    case VK_PHYSICAL_DEVICE_TYPE_OTHER: type = "(Other)\n"; break;
    case VK_PHYSICAL_DEVICE_TYPE_INTEGRATED_GPU: type = "(Integrated)\n"; break;
    case VK_PHYSICAL_DEVICE_TYPE_DISCRETE_GPU: type = "(Discrete)\n"; break;
    case VK_PHYSICAL_DEVICE_TYPE_VIRTUAL_GPU: type = "(Virtual)\n"; break;
    case VK_PHYSICAL_DEVICE_TYPE_CPU: type = "(CPU)\n"; break;
    default: type = "\n"; break;
    }

    _bs_infoF(
        "Physical device %d:\n"     \
        "  API: %d.%d.%d\n"         \
        "  Driver: %d.%d.%d\n"      \
        "  Name: %s %s", 
        i,
        VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion),
        VK_VERSION_MAJOR(properties.driverVersion), VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion),
        properties.deviceName, type);

}

static void _bs_preparePhysicalDevice() {
    bs_U32 num_devices = 0;
    vkEnumeratePhysicalDevices(_bs_instance_->instance, &num_devices, NULL);
    if (num_devices == 0) {
        _bs_critical(BS_CONSTANT_STRING("No GPU with Vulkan support was found\n"));
        return;
    }

    VkPhysicalDevice* devices = _bs_calloc(num_devices, sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(_bs_instance_->instance, &num_devices, devices);

    int chosen = 0;
    for(int i = 0; i < num_devices; i++) {
        chosen = i;
        _bs_logPhysicalDeviceInfo(i, devices[i]);
        _bs_instance_->physical_device = devices[i];
        if (_bs_queueFamily(BS_QUEUE_GRAPHICS_BIT) != -1)
            break;
        _bs_instance_->physical_device = VK_NULL_HANDLE;
    }

    _bs_free(devices);

    if (_bs_instance_->physical_device == VK_NULL_HANDLE) {
        _bs_warn(BS_CONSTANT_STRING("No GPU with graphics support was found\n"));
        _bs_instance_->physical_device = devices[chosen = 0];
    }

    _bs_infoF("Physical device %d was picked\n", chosen);
}

static void _bs_prepareLogicalDevice() {
    VkResult vk_result;

    // todo shouldnt this all be in the physical device creation
   /**
    Features & Extensions
    */
    VkPhysicalDeviceRayTracingPipelineFeaturesKHR rt_pipeline_features = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_FEATURES_KHR, .pNext = NULL };
    VkPhysicalDevice16BitStorageFeatures _16_bit_storage_features = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_16BIT_STORAGE_FEATURES, .pNext = NULL };
    VkPhysicalDeviceDynamicRenderingFeaturesKHR dynamic_rendering_features = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_DYNAMIC_RENDERING_FEATURES_KHR, .pNext = &_16_bit_storage_features };
    //VkPhysicalDeviceAccelerationStructureFeaturesKHR as_features = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_FEATURES_KHR, .pNext = &dynamic_rendering_features };
    //VkPhysicalDeviceVulkan11Features features11 = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_1_FEATURES, .pNext = &as_features };
    VkPhysicalDeviceVulkan12Features features12 = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_VULKAN_1_2_FEATURES, .pNext = &dynamic_rendering_features };
    VkPhysicalDeviceFeatures2 features2 = { .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_FEATURES_2, .pNext = &features12 };
    VkPhysicalDeviceFeatures* features = &features2.features;
    vkGetPhysicalDeviceFeatures2(_bs_instance_->physical_device, &features2);

   /**
    NVIDIA Aftermath
    */

    VkDeviceDiagnosticsConfigCreateInfoNV aftermath_ci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_DIAGNOSTICS_CONFIG_CREATE_INFO_NV,
        .flags = 
            VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_AUTOMATIC_CHECKPOINTS_BIT_NV |
            VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_RESOURCE_TRACKING_BIT_NV |
            VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_DEBUG_INFO_BIT_NV |
            VK_DEVICE_DIAGNOSTICS_CONFIG_ENABLE_SHADER_ERROR_REPORTING_BIT_NV,
    };

    if (!features->depthClamp) _bs_warnF("Depth clamp is not supported");
    if (!features->fillModeNonSolid) _bs_warnF("Fill mode non solid is not supported");
    if (!features->independentBlend) _bs_warnF("Independent blend is not supported");
    if (!features->shaderInt64) _bs_warnF("64 bit integers are not supported");
    if (!features->robustBufferAccess) _bs_warnF("Robust buffer access is not supported");
    _bs_features_.independent_blend = features->independentBlend;

    const char* extensions[] = {
        "VK_KHR_swapchain",
        "VK_KHR_dynamic_rendering",
        //
        "VK_KHR_shader_non_semantic_info",
        "VK_KHR_ray_tracing_pipeline",
        "VK_KHR_acceleration_structure",
        "VK_KHR_deferred_host_operations",
        "VK_EXT_descriptor_indexing",
       // "VK_KHR_get_physical_device_properties2",
        "VK_KHR_shader_float_controls",
        "VK_KHR_spirv_1_4",
#ifdef _DEBUG
      //  "VK_NV_device_diagnostics_config"
      //  "VK_NV_ray_tracing_validation",
#endif
    };
    int extensions_count = sizeof(extensions) / sizeof(const char*);
    bool supported_extensions[sizeof(extensions) / sizeof(const char*)] = { 0 };

    bs_U32 total_extensions_count = 0;
    vk_result = vkEnumerateDeviceExtensionProperties(_bs_instance_->physical_device, NULL, &total_extensions_count, NULL);
    if (vk_result != VK_SUCCESS) {
        BS_WARN_VULKAN_ERROR("vkEnumerateDeviceExtensionProperties", vk_result, "");
    }

    VkExtensionProperties* props = _bs_calloc(total_extensions_count, sizeof(VkExtensionProperties));
    vk_result = vkEnumerateDeviceExtensionProperties(_bs_instance_->physical_device, NULL, &total_extensions_count, props);
    if (vk_result != VK_SUCCESS) {
        BS_WARN_VULKAN_ERROR("vkEnumerateDeviceExtensionProperties", vk_result, "");
    }

    for (int i = 0; i < extensions_count; i++) {
        for (int j = 0; j < total_extensions_count; j++) {
            if (strcmp(props[j].extensionName, extensions[i]) == 0) {
                supported_extensions[i] = true;
                break;
            }
        }
    }

    //_bs_features.ray_tracing =
    //    as_features.accelerationStructure;

    for (int i = 0; i < extensions_count; i++) {
        if (!supported_extensions[i]) {
            if (_bs_features_.ray_tracing && i >= 1 && i <= 8) // todo something about this
                _bs_features_.ray_tracing = false;

            _bs_warnF("Extension \"%s\" is not supported\n", extensions[i]);
        }
    }

    _bs_free(props);

   /**
    Properties
    */
    VkPhysicalDeviceAccelerationStructurePropertiesKHR accel_struct_properties = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_ACCELERATION_STRUCTURE_PROPERTIES_KHR,
    };

    VkPhysicalDeviceRayTracingPipelinePropertiesKHR ray_tracing_pipeline_properties = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_RAY_TRACING_PIPELINE_PROPERTIES_KHR,
        .pNext = &accel_struct_properties
    };

    VkPhysicalDeviceProperties2 device_properties = {
        .sType = VK_STRUCTURE_TYPE_PHYSICAL_DEVICE_PROPERTIES_2,
       // .pNext = &ray_tracing_pipeline_properties
    };

    vkGetPhysicalDeviceProperties2(_bs_instance_->physical_device, &device_properties);

    _bs_props_.shader_group_handle_size = ray_tracing_pipeline_properties.shaderGroupHandleSize;
    _bs_props_.shader_group_base_alignment = ray_tracing_pipeline_properties.shaderGroupBaseAlignment;
    _bs_props_.min_acceleration_structure_scratch_offset_alignment = accel_struct_properties.minAccelerationStructureScratchOffsetAlignment;
    /**
    Creation
    */
    float queue_priority = 1.0;
    VkDeviceQueueCreateInfo queue_ci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = _bs_queueFamily(BS_QUEUE_GRAPHICS_BIT),
        .queueCount = 1,
        .pQueuePriorities = &queue_priority,
    };

    VkDeviceCreateInfo ci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_CREATE_INFO,
        .pNext = &features2,
        .pQueueCreateInfos = &queue_ci,
        .queueCreateInfoCount = 1,
      //  .pEnabledFeatures = features,
        .enabledExtensionCount = extensions_count,
        .ppEnabledExtensionNames = extensions,
        .ppEnabledLayerNames = validation_layers,
        .enabledLayerCount = _bs_args_.use_validation_layers ? sizeof(validation_layers) / sizeof(const char*) : 0,
    };

    vk_result = vkCreateDevice(_bs_instance_->physical_device, &ci, NULL, &_bs_instance_->device);
    if (vk_result != VK_SUCCESS) {
        BS_CRITICAL_VULKAN_ERROR("vkCreateDevice", vk_result, "");
    }

    //_bs_nameHandlef((bs_U64)_bs_instance->_.graphics_queue, VK_OBJECT_TYPE_QUEUE, "graphics queue");
    //_bs_nameHandlef((bs_U64)_bs_instance->_.compute_queue, VK_OBJECT_TYPE_QUEUE, "compute queue");
}

static void _bs_prepareCommands() {
    VkCommandPoolCreateInfo pool_ci = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = _bs_queueFamily(BS_QUEUE_GRAPHICS_BIT),
    };

    VkResult result = vkCreateCommandPool(_bs_instance_->device, &pool_ci, NULL, &_bs_instance_->command_pool);
    BS_CRITICAL_VULKAN_ERROR("vkCreateCommandPool", result, "");
}

BSAPI bs_Args* _bs_arguments() {
    return &_bs_args_;
}

BSAPI void _bs_parseArgs(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        if (strcmp(argv[i], "--color-log") == 0) _bs_args_.color_log = true;
        else if (strcmp(argv[i], "--use-lisk") == 0) _bs_args_.use_lisk = true;
        else if (strcmp(argv[i], "--use-validation-layers") == 0) _bs_args_.use_validation_layers = true;
        else if (strcmp(argv[i], "--send-bugs") == 0) _bs_args_.send_bugs = true;
        else if (strcmp(argv[i], "--track-changes") == 0) _bs_args_.track_changes = true;
    }
}

BSAPI void _bs_queryProcedures(bs_Procedure* procedures, int count, void* dll_handle, unsigned char* destination) {
#define BS_STRING_GEN_2(TYPE, FUNC, ...) { .size = sizeof(TYPE), .func = #FUNC, __VA_OPT__(.is_required = __VA_ARGS__) },

    for (int i = 0; i < count; i++) {
        void* data = dll_handle == 0 ?
            (void*)vkGetDeviceProcAddr(_bs_instance_->device, procedures[i].func) :
            (void*)GetProcAddress(dll_handle, procedures[i].func);

        if (data)
            memcpy(destination, &data, procedures[i].size);
        else if (procedures[i].is_required) {
            _bs_warnF("Failed to query procedure \"%s\"\n", procedures[i].func);
        }

        destination += procedures[i].size;
    }
}

void _bs_findExecutablePaths();
BSAPI void _bs_ini() {
    _bs_instance_ = _bs_calloc(1, sizeof(bs_Instance));

    _bs_configureAttribute("bs_Position", BS_FORMAT_R32_SFLOAT);
    _bs_configureAttribute("bs_Texture", BS_FORMAT_R32_SFLOAT);
    _bs_configureAttribute("bs_Color", BS_FORMAT_R8_UNORM);
    _bs_configureAttribute("bs_Normal", BS_FORMAT_R32_SFLOAT);
    _bs_configureAttribute("bs_Bone", BS_FORMAT_R32_SINT);
    _bs_configureAttribute("bs_Weight", BS_FORMAT_R32_SFLOAT);

   // _bs_wnd.fixed_time = 0.025;
    _bs_findExecutablePaths();
    _bs_prepareInstance();
    //_bs_prepareSurface();
    _bs_preparePhysicalDevice();
    _bs_prepareLogicalDevice();

    bs_Procedure procedures[] = { BS_FOREACH_PROC(BS_STRING_GEN_2) };
    _bs_queryProcedures(procedures, sizeof(procedures) / sizeof(*procedures), 0, &_bs_procs_);

    //_bs_prepareSwapchain();
    _bs_prepareCommands();
}

static int _bs_compareInt(const int* a, const int* b) {
    if (*a == *b) return 0;
    else if (*a < *b) return -1;
    else return 1;
}

BSAPI void _bs_load(
    bs_Callback load_resources)
{
    if (!_bs_instance_->single_times_queue) {
        bs_Object* object = BS_QUEUE(-1, 0, 0);
        if (_bs_queue(object, BS_QUEUE_TRANSFER_BIT | BS_QUEUE_COMPUTE_BIT | BS_QUEUE_SINGLE_TIMES_BIT) != BS_RESULT_OK) {
            _bs_critical(BS_CONSTANT_STRING("Failed to create single times queue\n"));
            return;
        }

        _bs_instance_->single_times_queue;
    }
    _bs_scope_.queue = _bs_instance_->single_times_queue;

    if (load_resources)
        load_resources();

    _bs_scope_.queue = NULL;
}

void _bs_setFunctions(const struct _bs_FunctionTable* table);
void _preval_bs_setFunctions(const struct _preval_bs_FunctionTable* table);

BSAPI void bs_setupTrampoline() {
    bs_FunctionTable definitions = _bs_getFunctions();
    bs_FunctionTable preval_definitions = _preval_bs_getFunctions();
    bs_FunctionTable val_definitions = _val_bs_getFunctions();

    _bs_setFunctions(&preval_definitions);
    _preval_bs_setFunctions(&val_definitions);
    _preval_bs_setFunctions(&val_definitions);
}