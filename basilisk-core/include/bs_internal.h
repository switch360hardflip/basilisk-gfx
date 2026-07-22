
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

#ifndef BS_INTERNAL_H
#define BS_INTERNAL_H

#include <bs_internal.gen.h>

#define BS_FOREACH_PROC(X) \
    X(PFN_vkCmdInsertDebugUtilsLabelEXT, vkCmdInsertDebugUtilsLabelEXT) \
    X(PFN_vkCmdBeginDebugUtilsLabelEXT, vkCmdBeginDebugUtilsLabelEXT) \
    X(PFN_vkCmdEndDebugUtilsLabelEXT, vkCmdEndDebugUtilsLabelEXT) \
    X(PFN_vkCmdBeginRenderingKHR, vkCmdBeginRenderingKHR) \
    X(PFN_vkCmdEndRenderingKHR, vkCmdEndRenderingKHR) \
    X(PFN_vkCmdTraceRaysKHR, vkCmdTraceRaysKHR) \
    X(PFN_vkGetAccelerationStructureBuildSizesKHR, vkGetAccelerationStructureBuildSizesKHR) \
    X(PFN_vkCreateAccelerationStructureKHR, vkCreateAccelerationStructureKHR) \
    X(PFN_vkCmdBuildAccelerationStructuresKHR, vkCmdBuildAccelerationStructuresKHR) \
    X(PFN_vkGetAccelerationStructureDeviceAddressKHR, vkGetAccelerationStructureDeviceAddressKHR) \
    X(PFN_vkGetRayTracingShaderGroupHandlesKHR, vkGetRayTracingShaderGroupHandlesKHR) \
    X(PFN_vkDestroyAccelerationStructureKHR, vkDestroyAccelerationStructureKHR) \
    X(PFN_vkCreateRayTracingPipelinesKHR, vkCreateRayTracingPipelinesKHR) \

#define BS_STRUCT_GEN(TYPE, FUNC, ...) TYPE FUNC;

typedef struct bs_Procs bs_Procs;
extern struct bs_Procs {
    BS_FOREACH_PROC(BS_STRUCT_GEN)
} _bs_procs_;

#ifdef _WIN32
#define BS_WARN_WIN32_PATH(function, path)                           \
    bs_warnF("%s at %s:%d: %s failed for \"%s\" (Win32 error %lu = \"%s\")\n", __func__, __FILE__, __LINE__, function, path, GetLastError(), bs_serializeWin32Error(GetLastError()))
#endif

#define BS_WARN_ERRNO_PATH(function, path)                           \
    bs_warnF("%s at %s:%d: %s failed for \"%s\" (errno %d = \"%s\")\n", __func__, __FILE__, __LINE__, function, path, errno, bs_serializeErrno())

#define BS_VALIDATE(condition, ret, format, ...)                     \
    if (!(condition)) {                                              \
        bs_warnF(BS_PRINT_COLOR("[CORE] [VAL]", BS_PRINT_RED) " %s " BS_PRINT_COLOR("in", BS_PRINT_CYAN) " %s " BS_PRINT_COLOR("at", BS_PRINT_CYAN) " %s:%d\n" __VA_OPT__(format) "\n", #condition, __func__, __FILE__, __LINE__ __VA_OPT__(,) __VA_ARGS__); \
        return ret;                                                  \
    }

#define BS_WARN_VULKAN_ERROR(function, code, format, ...)            \
    bs_warnF("%s at %s:%d: %s failed" __VA_OPT__(", ") format " (Vulkan result %d\n", __func__, __FILE__, __LINE__, function __VA_OPT__(,) __VA_ARGS__, code)

#define BS_CRITICAL_VULKAN_ERROR(function, code, format, ...)        \
    bs_warnF("%s at %s:%d: %s failed" __VA_OPT__(", ") format " (Vulkan result %d)\n", __func__, __FILE__, __LINE__, function __VA_OPT__(,) __VA_ARGS__, code)

#define BS_VALIDATE_OBJECT_TYPE(object, source_id, _return)          \
    BS_VALIDATE(((bs_ObjectSource*)bs_fetchUnit(bs_objectSources(), source_id))->type == source_id, _return,,)

#define BS_WARN_INVALID_MAGIC(resource_type, path)                   \
    bs_warnF("%s at %s:%d: Invalid magic for %s \"%s\"\n", __func__, __FILE__, __LINE__, resource_type, path)

#define BS_WARN_UNSUPPORTED_VERSION(resource_type, path)             \
    bs_warnF("%s at %s:%d: Unsupported version for %s \"%s\"\n", __func__, __FILE__, __LINE__, resource_type, path)


#endif