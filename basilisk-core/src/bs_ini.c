#include <windows.h>
#include <stdio.h>
#include <stdbool.h>
#include <assert.h>

#define VK_USE_PLATFORM_WIN32_KHR
#include <vulkan.h>
#include <vulkan/vulkan_core.h>

#include <basilisk-core.h>
#include <bs_internal.h>

const char* validation_layers[] = {
    "VK_LAYER_KHRONOS_validation"
};

bs_Args _bs_args = { 0 };
bs_Features _bs_features = { 0 };
bs_Props _bs_props = { 0 };
bs_Settings _bs_settings = { 
    .frames_in_flight = 2,
    .frames_in_flight_max = 3,
};
bs_Procs _bs_procs_ = { 0 };
bs_Config _bs_config = {
    .attributes.unit_size = sizeof(bs_AttributeType),
};

bs_Instance* bs_instance() { return _bs_instance; }
bs_Args* bs_args() { return &_bs_args; }
bs_Features* bs_features() { return &_bs_features; }
bs_Props* bs_props() { return &_bs_props; }
bs_Settings* bs_settings() { return &_bs_settings; }
bs_Config* bs_config() { return &_bs_config; }

void bsi_nameHandle(bs_U64 handle, bs_U32 type, const char* name) {
    PFN_vkSetDebugUtilsObjectNameEXT pfn_vkSetDebugUtilsObjectNameEXT = (PFN_vkSetDebugUtilsObjectNameEXT)
        vkGetDeviceProcAddr(_bs_instance_->device, "vkSetDebugUtilsObjectNameEXT");

    const VkDebugUtilsObjectNameInfoEXT name_i = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_OBJECT_NAME_INFO_EXT,
        .pNext = NULL,
        .objectType = type,
        .objectHandle = handle,
        .pObjectName = name,
    };

    pfn_vkSetDebugUtilsObjectNameEXT(_bs_instance_->device, &name_i);
}

void bsi_nameHandleF(bs_U64 handle, bs_U32 type, const char* format, ...) {
	va_list argptr;
	va_start(argptr, format);

    char name[256];
	vsprintf(name, format, argptr);

    bsi_nameHandle(handle, type, name);

	va_end(argptr);
}

static const char* bs_vulkanObjectName(VkObjectType type) {
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

static VKAPI_ATTR VkBool32 VKAPI_CALL bs_debugCallback(
    VkDebugUtilsMessageSeverityFlagBitsEXT severity,
    VkDebugUtilsMessageTypeFlagsEXT type,
    const VkDebugUtilsMessengerCallbackDataEXT* data,
    void* param)
{
    static bs_String* message;


    if (severity & VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT) {
        message = bs_stringF(message, "Message name: %s\n", data->pMessageIdName ? data->pMessageIdName : "N/A\n");
        message = bs_appendStringF(message, "Message: %s\n", data->pMessage);

        if (data->objectCount > 0) {
            for (uint32_t i = 0; i < data->objectCount; i++) {
                const VkDebugUtilsObjectNameInfoEXT* obj = &data->pObjects[i];
                message = bs_appendStringF(message, "Object %d:\n", i);
                message = bs_appendStringF(message, "  Handle: 0x%llx\n", (unsigned long long)obj->objectHandle);
                message = bs_appendStringF(message, "  Type:   %s\n", bs_vulkanObjectName(obj->objectType));
                if (obj->pObjectName)
                    message = bs_appendStringF(message, "  Name:   %s\n", obj->pObjectName);
            }
        }
        bs_throwBasiliskF(BSX_VALIDATION, "%s", message->value);
    }
    else {
        printf("%s\n", data->pMessage);
    }

    // bs_free(message);

    return VK_FALSE;
}

static VKAPI_ATTR VkBool32 VKAPI_CALL bs_reportCallback(
    VkDebugReportFlagsEXT flags,
    VkDebugReportObjectTypeEXT type,
    uint64_t object,
    size_t location,
    int32_t code,
    const char* prefix,
    const char* message,
    void* param) {

 //   bs_infoF("%s\n", message);
    return VK_FALSE;
}

void bs_cleanup() {
    vkDestroyDevice(_bs_instance_->device, NULL);
    vkDestroySurfaceKHR(_bs_instance_->instance, _bs_instance_->surface, NULL);
    vkDestroyInstance(_bs_instance_->instance, NULL);

    //glfwDestroyWindow(window);r
    //glfwTerminate();
}

static bool bs_checkValidationLayerSupport() {
    bs_U32 num_layers;
    vkEnumerateInstanceLayerProperties(&num_layers, NULL);

    VkLayerProperties* layers = bs_malloc(num_layers * sizeof(VkLayerProperties));
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
            bs_warnF("Vulkan validation layer %s is not supported\n", validation_layers[i]);
            found_all = false;
        }
    }

    bs_free(layers);
    return found_all;
}

static inline bool bs_addInstanceExtension(const char** extensions, bs_U32* extensions_count, const char* name, VkExtensionProperties* available_extensions, bs_U32 available_extensions_count) {
    for (bs_U32 i = 0; i < available_extensions_count; i++) {
        if (strcmp(name, available_extensions[i].extensionName) == 0) {
            extensions[(*extensions_count)++] = available_extensions[i].extensionName;
            return true;
        }
    }

    return false;
}

static void bs_prepareInstance() {

    if (_bs_args.use_validation_layers)
        _bs_args.use_validation_layers = bs_checkValidationLayerSupport();

    const char* extensions[16];
    bs_U32 extensions_count = 0;

    bs_U32 available_extensions_count = 0;
    vkEnumerateInstanceExtensionProperties(NULL, &available_extensions_count, NULL);
    VkExtensionProperties* available_extensions = malloc(available_extensions_count * sizeof(VkExtensionProperties));
    vkEnumerateInstanceExtensionProperties(NULL, &available_extensions_count, available_extensions);

#define BS_ADD_INSTANCE_EXTENSION(name) \
    bs_addInstanceExtension(extensions, &extensions_count, name, available_extensions, available_extensions_count)

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
        bs_warnF("Only off-screen rendering is available\n");
        _bs_instance_->extensions.surface_type = BS_SURFACE_TYPE_HEADLESS;
    }
    else
        return bs_throwBasiliskF(BSX_NOT_SUPPORTED, "No instance surface extension found");
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
        .enabledLayerCount = _bs_args.use_validation_layers ? sizeof(validation_layers) / sizeof(const char*) : 0,
     //  .pNext = &features,
    };

    bs_throwVulkan(vkCreateInstance(&ci, NULL, &_bs_instance_->instance));
    free(available_extensions);

    VkDebugUtilsMessengerEXT messenger = { 0 };
    const VkDebugUtilsMessengerCreateInfoEXT debug_ci = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_UTILS_MESSENGER_CREATE_INFO_EXT,
        .messageSeverity = VK_DEBUG_UTILS_MESSAGE_SEVERITY_ERROR_BIT_EXT,
        .messageType = VK_DEBUG_UTILS_MESSAGE_TYPE_GENERAL_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_VALIDATION_BIT_EXT | VK_DEBUG_UTILS_MESSAGE_TYPE_PERFORMANCE_BIT_EXT,
        .pfnUserCallback = bs_debugCallback,
    };

    PFN_vkCreateDebugUtilsMessengerEXT create_messenger = vkGetInstanceProcAddr(_bs_instance_->instance, "vkCreateDebugUtilsMessengerEXT");
    PFN_vkCreateDebugUtilsMessengerEXT create_reporter = vkGetInstanceProcAddr(_bs_instance_->instance, "vkCreateDebugReportCallbackEXT");

    if (create_messenger)
        create_messenger(_bs_instance_->instance, &debug_ci, NULL, &messenger);

    VkDebugReportCallbackCreateInfoEXT report_ci = {
        .sType = VK_STRUCTURE_TYPE_DEBUG_REPORT_CALLBACK_CREATE_INFO_EXT,
        .flags = VK_DEBUG_REPORT_ERROR_BIT_EXT,
        .pfnCallback = bs_reportCallback,
    };

    VkDebugReportCallbackEXT reporter = { 0 };
    if (create_reporter)
        create_reporter(_bs_instance_->instance, &report_ci, NULL, &reporter);

}

static void bs_prepareSurface() {
    VkResult result = VK_SUCCESS;

    if (_bs_instance_->extensions.surface_type == BS_SURFACE_TYPE_WIN32) {
        VkWin32SurfaceCreateInfoKHR ci = {
            .sType = VK_STRUCTURE_TYPE_WIN32_SURFACE_CREATE_INFO_KHR,
            .hinstance = GetModuleHandle(0),
            .hwnd = _bs_wnd.hwnd,
        };

        result = vkCreateWin32SurfaceKHR(_bs_instance_->instance, &ci, NULL, &_bs_instance_->surface);
    }
    else if (_bs_instance_->extensions.surface_type == BS_SURFACE_TYPE_HEADLESS) {
        VkHeadlessSurfaceCreateInfoEXT ci = {
            .sType = VK_STRUCTURE_TYPE_HEADLESS_SURFACE_CREATE_INFO_EXT,
        };

        result = vkCreateHeadlessSurfaceEXT(_bs_instance_->instance, &ci, NULL, &_bs_instance_->surface);
    }
    else
        bs_throwBasiliskF(BSX_NOT_SUPPORTED, "Surface type %d", _bs_instance_->extensions.surface_type);

    if (result != VK_SUCCESS)
        bs_throwVulkan(result);
}

static inline void bs_logPhysicalDeviceInfo(int i, VkPhysicalDevice device) {
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

    bs_infoF(
        "Physical device %d:\n"     \
        "  API: %d.%d.%d\n"         \
        "  Driver: %d.%d.%d\n"      \
        "  Name: %s %s", 
        i,
        VK_VERSION_MAJOR(properties.apiVersion), VK_VERSION_MINOR(properties.apiVersion), VK_VERSION_PATCH(properties.apiVersion),
        VK_VERSION_MAJOR(properties.driverVersion), VK_VERSION_MINOR(properties.driverVersion), VK_VERSION_PATCH(properties.driverVersion),
        properties.deviceName, type);

}

static void bs_preparePhysicalDevice() {
    bs_U32 num_devices = 0;
    vkEnumeratePhysicalDevices(_bs_instance_->instance, &num_devices, NULL);
    if (num_devices == 0)
	    bs_throwBasiliskF(BSXI_INTERNAL | BSX_FAILED_TO_QUERY, "No GPU with Vulkan support was found!\n");

    VkPhysicalDevice* devices = bs_calloc(num_devices, sizeof(VkPhysicalDevice));
    vkEnumeratePhysicalDevices(_bs_instance_->instance, &num_devices, devices);

    int chosen = 0;
    for(int i = 0; i < num_devices; i++) {
        chosen = i;
        bs_logPhysicalDeviceInfo(i, devices[i]);
        _bs_instance_->physical_device = devices[i];
        if (bs_queueFamily(BS_QUEUE_GRAPHICS_BIT) != -1)
            break;
        _bs_instance_->physical_device = VK_NULL_HANDLE;
    }

    bs_free(devices);

    if (_bs_instance_->physical_device == VK_NULL_HANDLE)
        bs_throwBasilisk(BSXI_INTERNAL | BSX_NO_GPU);

    bs_infoF("Physical device %d was picked\n", chosen);
}

static void bs_prepareLogicalDevice() {
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

    if (!features->depthClamp) bs_warnF("Depth clamp is not supported");
    if (!features->fillModeNonSolid) bs_warnF("Fill mode non solid is not supported");
    if (!features->independentBlend) bs_warnF("Independent blend is not supported");
    if (!features->shaderInt64) bs_warnF("64 bit integers are not supported");
    if (!features->robustBufferAccess) bs_warnF("Robust buffer access is not supported");
    _bs_features.independent_blend = features->independentBlend;

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
    bs_throwVulkan(vkEnumerateDeviceExtensionProperties(_bs_instance_->physical_device, NULL, &total_extensions_count, NULL));
    VkExtensionProperties* props = bs_calloc(total_extensions_count, sizeof(VkExtensionProperties));
    bs_throwVulkan(vkEnumerateDeviceExtensionProperties(_bs_instance_->physical_device, NULL, &total_extensions_count, props));

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
            if (_bs_features.ray_tracing && i >= 1 && i <= 8) // todo something about this
                _bs_features.ray_tracing = false;

            bs_warnF("Extension \"%s\" is not supported!\n", extensions[i]);
        }
    }

    bs_free(props);

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

    _bs_props.shader_group_handle_size = ray_tracing_pipeline_properties.shaderGroupHandleSize;
    _bs_props.shader_group_base_alignment = ray_tracing_pipeline_properties.shaderGroupBaseAlignment;
    _bs_props.min_acceleration_structure_scratch_offset_alignment = accel_struct_properties.minAccelerationStructureScratchOffsetAlignment;
    /**
    Creation
    */
    float queue_priority = 1.0;
    VkDeviceQueueCreateInfo queue_ci = {
        .sType = VK_STRUCTURE_TYPE_DEVICE_QUEUE_CREATE_INFO,
        .queueFamilyIndex = bs_queueFamily(BS_QUEUE_GRAPHICS_BIT),
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
        .enabledLayerCount = _bs_args.use_validation_layers ? sizeof(validation_layers) / sizeof(const char*) : 0,
    };

    bs_throwVulkan(vkCreateDevice(_bs_instance_->physical_device, &ci, NULL, &_bs_instance_->device));

    //bs_nameHandlef((bs_U64)bs_instance->_.graphics_queue, VK_OBJECT_TYPE_QUEUE, "graphics queue");
    //bs_nameHandlef((bs_U64)bs_instance->_.compute_queue, VK_OBJECT_TYPE_QUEUE, "compute queue");
}

static void bs_prepareCommands() {
    VkCommandPoolCreateInfo pool_ci = {
        .sType = VK_STRUCTURE_TYPE_COMMAND_POOL_CREATE_INFO,
        .flags = VK_COMMAND_POOL_CREATE_RESET_COMMAND_BUFFER_BIT,
        .queueFamilyIndex = bs_queueFamily(BS_QUEUE_GRAPHICS_BIT),
    };

    bs_throwVulkan(vkCreateCommandPool(_bs_instance_->device, &pool_ci, NULL, &_bs_instance_->command_pool));
}

static VkSurfaceFormatKHR bs_querySwapchainFormat(bs_U32 num_candidates, ...) {
    va_list args;
    va_start(args, num_candidates);

    bs_U32 num_formats = 0;
    vkGetPhysicalDeviceSurfaceFormatsKHR(_bs_instance_->physical_device, _bs_instance_->surface, &num_formats, NULL);
    VkSurfaceFormatKHR* formats = bs_malloc(num_formats * sizeof(VkSurfaceFormatKHR));
    vkGetPhysicalDeviceSurfaceFormatsKHR(_bs_instance_->physical_device, _bs_instance_->surface, &num_formats, formats);

    for (int i = 0; i < num_candidates; i++) {
        VkFormat candidate = va_arg(args, VkFormat);

        for (int j = 0; j < num_formats; j++) {
            if (candidate == formats[j].format) {
                VkSurfaceFormatKHR result = formats[j];
                bs_free(formats);
                return result;
            }
        }
    }

    va_end(args);

    bs_free(formats);
    for (int i = 0; i < num_formats; i++)
        bs_infoF("%d\n", formats[i].format);
    bs_throwBasiliskF(BSXI_INTERNAL | BSX_FAILED_TO_QUERY, "Swapchain format");

    return (VkSurfaceFormatKHR) { 0 };
}

static VkPresentModeKHR bs_querySwapchainMode(bs_U32 num_candidates, ...) {
    va_list args;
    va_start(args, num_candidates);

    bs_U32 num_modes = 0;
    VkPresentModeKHR result = 0;
    vkGetPhysicalDeviceSurfacePresentModesKHR(_bs_instance_->physical_device, _bs_instance_->surface, &num_modes, NULL);
    VkPresentModeKHR* modes = bs_malloc(num_modes * sizeof(VkPresentModeKHR));
    vkGetPhysicalDeviceSurfacePresentModesKHR(_bs_instance_->physical_device, _bs_instance_->surface, &num_modes, modes);

    for (int i = 0; i < num_candidates; i++) {
        VkPresentModeKHR candidate = va_arg(args, VkPresentModeKHR);

        for (int j = 0; j < num_modes; j++) {
            VkPresentModeKHR mode = modes[j];
            if (candidate == mode) {
                bs_free(modes);
                return result;
            }
        }
    }

    va_end(args);
    bs_free(modes);
    bs_throwBasiliskF(BSXI_INTERNAL | BSX_FAILED_TO_QUERY, "Swapchain mode");
    return result;
}

void bs_prepareSwapchain() {
    VkSurfaceCapabilitiesKHR capabilities;
    VkSurfaceFormatKHR surface_format = bs_querySwapchainFormat(4, 
        VK_FORMAT_R8G8B8A8_UNORM, VK_FORMAT_B8G8R8A8_UNORM, VK_FORMAT_R8G8B8A8_SRGB, VK_FORMAT_B8G8R8A8_SRGB);
    vkGetPhysicalDeviceSurfaceCapabilitiesKHR(_bs_instance_->physical_device, _bs_instance_->surface, &capabilities);

    const bool same_family = true; // @todo
    // todo what? ofc i forgot this you fucking idiot

    bs_Swapchain swapchain = {
        .flags = (capabilities.supportedUsageFlags & VK_IMAGE_USAGE_TRANSFER_SRC_BIT) ? BS_SWAPCHAIN_BITS_CAN_COPY : 0,
        .color_space = surface_format.colorSpace,
        .image = _bs_swapchain ? _bs_swapchain->image : NULL,
    };

    bs_ivec2 resolution = bs_iv2(capabilities.minImageExtent.width, capabilities.minImageExtent.height);
    bs_Image image = {
        .flags = BS_IMAGE_SWAPS_BIT,
        .format = surface_format.format,
        .dim = resolution
    };

    // todo frames in flight = 1

    _bs_settings_.buffer_count_min = capabilities.minImageCount;
    _bs_settings_.frames_in_flight_max = bs_iclamp(_bs_settings_.frames_in_flight_max, capabilities.minImageCount, capabilities.maxImageCount);

    VkSwapchainCreateInfoKHR swapchain_ci = {
        .sType = VK_STRUCTURE_TYPE_SWAPCHAIN_CREATE_INFO_KHR,
        .surface = _bs_instance_->surface,
        .imageArrayLayers = 1,
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .minImageCount = bs_clamp(_bs_settings_.frames_in_flight, _bs_settings_.buffer_count_min, _bs_settings_.frames_in_flight_max),
        .imageExtent = { resolution.x, resolution.y },
        .imageUsage = VK_IMAGE_USAGE_COLOR_ATTACHMENT_BIT,
        .imageSharingMode = same_family ? VK_SHARING_MODE_EXCLUSIVE : VK_SHARING_MODE_CONCURRENT,
        .queueFamilyIndexCount = same_family ? 0 : 2,
        .pQueueFamilyIndices = same_family ? NULL : NULL,
        .compositeAlpha = VK_COMPOSITE_ALPHA_OPAQUE_BIT_KHR,
        .clipped = VK_TRUE,
        .preTransform = capabilities.currentTransform,
        .presentMode = bs_querySwapchainMode(4, VK_PRESENT_MODE_IMMEDIATE_KHR, VK_PRESENT_MODE_FIFO_KHR, VK_PRESENT_MODE_MAILBOX_KHR, VK_PRESENT_MODE_FIFO_RELAXED_KHR),
        .imageFormat = surface_format.format,
        .imageColorSpace = surface_format.colorSpace,
    };

    if (swapchain_ci.imageFormat == VK_FORMAT_UNDEFINED)
        bs_throwBasilisk(BSXI_INTERNAL | BSX_INVALID_TYPE);
    bs_throwVulkan(vkCreateSwapchainKHR(_bs_instance_->device, &swapchain_ci, NULL, &swapchain.swapchain));


   /**
    Swapchain images
    */
    int swapchain_image_count = BS_MAX(_bs_settings_.frames_in_flight, _bs_settings_.buffer_count_min);
    VkImage images[3];
    vkGetSwapchainImagesKHR(_bs_instance_->device, swapchain.swapchain, &swapchain_image_count, images);
    bs_infoF("Swapchain\n  Format: %d\n  Mode: %d\n  Images: %d\n", swapchain_ci.imageFormat, swapchain_ci.presentMode, _bs_settings_.frames_in_flight);

    // TODO: use new object system
    if (!_bs_swapchain_) {
        swapchain.image = BS_OBJECT(bs_Image, -1, 0, swapchain_image_count, BS_OBJECT_HAS_SWAPS_BIT);
        *swapchain.image->image = image;
        bs_U64 size = sizeof(bs_Swapchain) + BS_SWAP_SIZE(bs_Swapchain) * swapchain_image_count;
        _bs_swapchain_ = _bs_swapchain_ ? _bs_swapchain_ : bs_malloc(size);
    }
    else
        *swapchain.image->image = image;
    memcpy(_bs_swapchain_, &swapchain, sizeof(bs_Swapchain));

   /**
    Swapchain image views
    */
    for (int i = 0; i < swapchain_image_count; i++) {
        _bs_swapchain_->image->image->_[i].vk_image = images[i];

        VkImageViewCreateInfo image_view_ci = {
            .sType = VK_STRUCTURE_TYPE_IMAGE_VIEW_CREATE_INFO,
            .image = _bs_swapchain_->image->image->_[i].vk_image,
            .viewType = VK_IMAGE_VIEW_TYPE_2D,
            .format = surface_format.format,
            .subresourceRange.aspectMask = VK_IMAGE_ASPECT_COLOR_BIT,
            .subresourceRange.levelCount = 1,
            .subresourceRange.layerCount = 1,
        };

        bs_throwVulkan(vkCreateImageView(_bs_instance_->device, &image_view_ci, NULL, &_bs_swapchain_->image->image->_[i].view));
    }

   /**
    Swapchain semaphores
    */
    VkSemaphoreCreateInfo semaphore_ci = {
        .sType = VK_STRUCTURE_TYPE_SEMAPHORE_CREATE_INFO,
    };

    for (int i = 0; i < swapchain_image_count; i++) {
        bs_throwVulkan(vkCreateSemaphore(_bs_instance_->device, &semaphore_ci, NULL, &_bs_swapchain_->_[i].semaphore));
    }
}


bs_Args* bs_arguments() {
    return &_bs_args;
}

void bs_parseArgs(int argc, char* argv[]) {
    for (int i = 0; i < argc; i++) {
        if      (strcmp(argv[i], "--cmd-log") == 0) _bs_args.cmd_log = true;
        else if (strcmp(argv[i], "--color-log") == 0) _bs_args.color_log = true;
        else if (strcmp(argv[i], "--use-lisk") == 0) _bs_args.use_lisk = true;
        else if (strcmp(argv[i], "--use-validation-layers") == 0) _bs_args.use_validation_layers = true;
        else if (strcmp(argv[i], "--skip-log-info") == 0) _bs_args.skip_log_info = true;
        else if (strcmp(argv[i], "--send-bugs") == 0) _bs_args.send_bugs = true;
        else if (strcmp(argv[i], "--track-changes") == 0) _bs_args.track_changes = true;
    }
}

#define BS_REQUIRE_PROCEDURE(name) \
    if (!_bs_procs_.##name##) { \
        bs_throwBasiliskF(BSX_FAILED_TO_QUERY, #name); \
    }

void bs_queryProcedures(bs_Procedure* procedures, int count, HMODULE dll_handle, unsigned char* destination) {
#define BS_STRING_GEN_2(TYPE, FUNC, ...) { .size = sizeof(TYPE), .func = #FUNC, __VA_OPT__(.is_required = __VA_ARGS__) },

    for (int i = 0; i < count; i++) {
        void* data = dll_handle == 0 ?
            vkGetDeviceProcAddr(_bs_instance_->device, procedures[i].func) :
            GetProcAddress(dll_handle, procedures[i].func);

        if (data)
            memcpy(destination, &data, procedures[i].size);
        else if (procedures[i].is_required)
            bs_throwBasiliskF(BSX_FAILED_TO_QUERY, "Procedure \"%s\"", procedures[i].func);

        destination += procedures[i].size;
    }
}

void bs_findExecutablePaths();
void bs_ini() {
    bs_configureAttribute("bs_Position", BS_FORMAT_R32_SFLOAT);
    bs_configureAttribute("bs_Texture", BS_FORMAT_R32_SFLOAT);
    bs_configureAttribute("bs_Color", BS_FORMAT_R8_UNORM);
    bs_configureAttribute("bs_Normal", BS_FORMAT_R32_SFLOAT);
    bs_configureAttribute("bs_Bone", BS_FORMAT_R32_SINT);
    bs_configureAttribute("bs_Weight", BS_FORMAT_R32_SFLOAT);

    _bs_wnd.fixed_time = 0.025;
    bs_findExecutablePaths();
    bs_prepareInstance();
    bs_prepareSurface();
    bs_preparePhysicalDevice();
    bs_prepareLogicalDevice();

    bs_Procedure procedures[] = { BS_FOREACH_PROC(BS_STRING_GEN_2) };
    bs_queryProcedures(procedures, sizeof(procedures) / sizeof(*procedures), 0, &_bs_procs_);

    bs_prepareSwapchain();
    bs_prepareCommands();
}

static int bs_compareInt(const int* a, const int* b) {
    if (*a == *b) return 0;
    else if (*a < *b) return -1;
    else return 1;
}

void bs_load(
    bs_Callback load_resources)
{
    if (!_bs_instance_->single_times_queue)
        _bs_instance_->single_times_queue = bs_queue(BS_QUEUE(-1, 0, 0), BS_QUEUE_TRANSFER_BIT | BS_QUEUE_COMPUTE_BIT | BS_QUEUE_SINGLE_TIMES_BIT)->queue;
    _bs_scope_.queue = _bs_instance_->single_times_queue;

    if (load_resources)
        load_resources();

    _bs_scope_.queue = NULL;
}