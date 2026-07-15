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

#endif