cmake_minimum_required(VERSION 3.23.2)
project(basilisk-mod)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES "src/*.c")

file(GLOB TYPE_FILES "src/types/*.*")

file(GLOB UI_FILES "src/ui/*.*")

file(GLOB UI_PREFAB_FILES "src/ui/prefab/*.*")
file(GLOB UI_PRIMITIVE_FILES "src/ui/primitive/*.*")
file(GLOB UI_INSTANCE_FILES "src/ui/instance/*.*")
file(GLOB UI_TYPE_FILES "src/ui/type/*.*")
file(GLOB UI_CONTEXT_FILES "src/ui/context/*.*")
file(GLOB UI_MATERIAL_FILES "src/ui/material/*.*")
file(GLOB UI_TILE_FILES "src/ui/tile/*.*")
file(GLOB UI_GRID_FILES "src/ui/grid/*.*")
file(GLOB UI_SIDE_FILES "src/ui/side/*.*")
file(GLOB UI_LIGHT_FILES "src/ui/light/*.*")

file(GLOB VS_SHADER_FILES "resources/shaders/vert/*.*")
file(GLOB FS_SHADER_FILES "resources/shaders/frag/*.*")

file(GLOB_RECURSE HEADER_FILES "include/*.h")
file(GLOB PROJECT_FILES "./*" LIST_DIRECTORIES false)

add_library(${PROJECT_NAME} SHARED
    ${SOURCE_FILES}
    ${HEADER_FILES}

    ${UI_FILES}

    ${UI_PREFAB_FILES}
    ${UI_PRIMITIVE_FILES}
    ${UI_INSTANCE_FILESs}
    ${UI_MATERIAL_FILES}
    ${UI_TYPE_FILES}
    ${UI_CONTEXT_FILES}
    ${UI_TILE_FILES}
    ${UI_GRID_FILES}
    ${UI_SIDE_FILES}
    ${UI_LIGHT_FILES}

    ${VS_SHADER_FILES}
    ${FS_SHADER_FILES}

    ${TYPE_FILES}
)

source_group("Views" FILES ${VIEWS})
source_group("Source Files" FILES ${SOURCE_FILES})

source_group("Shaders/vert" FILES ${VS_SHADER_FILES})
source_group("Shaders/frag" FILES ${FS_SHADER_FILES})

source_group("Source Files/UI" FILES ${UI_FILES})
source_group("Source Files/UI/Prefab" FILES ${UI_PREFAB_FILES})
source_group("Source Files/UI/Primitive" FILES ${UI_PRIMITIVE_FILES})
source_group("Source Files/UI/Instance" FILES ${UI_INSTANCE_FILESs})
source_group("Source Files/UI/Light" FILES ${UI_LIGHT_FILES})
source_group("Source Files/UI/Tile" FILES ${UI_TILE_FILES})
source_group("Source Files/UI/Type" FILES ${UI_TYPE_FILES})
source_group("Source Files/UI/Material" FILES ${UI_MATERIAL_FILES})
source_group("Source Files/UI/Context" FILES ${UI_CONTEXT_FILES})
source_group("Source Files/UI/Grid" FILES ${UI_GRID_FILES})
source_group("Source Files/UI/Side" FILES ${UI_SIDE_FILES})

source_group("Source Files/Types" FILES ${TYPE_FILES})
source_group("Project Files" FILES ${PROJECT_FILES})
source_group("Headers" FILES ${HEADER_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/basilisk-core/include
    ${CMAKE_SOURCE_DIR}/basilisk-steam/include
    ${CMAKE_SOURCE_DIR}/basilisk-gfx/include
    ${CMAKE_SOURCE_DIR}/basilisk-gfx/src
    ${CMAKE_SOURCE_DIR}/basilisk-mod/include
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/GFSDK_Aftermath_Lib
    ${CMAKE_SOURCE_DIR}/external/glslang
    ${CMAKE_SOURCE_DIR}/external/SPIRV-Cross
    include/
    src/
    ${CMAKE_SOURCE_DIR}/include/objects/
    levels/
)

target_link_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/external/glslang/build/glslang/Release
    ${CMAKE_SOURCE_DIR}/external/glslang/build/SPIRV/Release
    ${CMAKE_SOURCE_DIR}/external/SPIRV-Cross/build/Release
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    basilisk-core
    basilisk-gfx
    basilisk-steam
    vulkan-1 
    steam_api64 
    dwmapi
    GFSDK_Aftermath_Lib.x64
    GenericCodeGen
    glslangd optimized glslang
    glslang-default-resource-limitsd optimized glslang-default-resource-limits
    MachineIndependentd  optimized MachineIndependent
    SPIRVd optimized SPIRV
    spirv-cross-cd optimized spirv-cross-c
    spirv-cross-cored optimized spirv-cross-core
    spirv-cross-glsld optimized spirv-cross-glsl
)

SET(COPY_BASILISK_MOD_DLL_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-mod/Debug/basilisk-mod.dll" "${CMAKE_SOURCE_DIR}/build/${BS_APP_NAME}/Debug/")
SET(COPY_BASILISK_MOD_DLL_RELEASE ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-mod/Release/basilisk-mod.dll" "${CMAKE_SOURCE_DIR}/build/${BS_APP_NAME}/Release")
SET(COPY_BASILISK_MOD_DLL_RELEASE_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-mod/RelWithDebInfo/basilisk-mod.dll" "${CMAKE_SOURCE_DIR}/build/${BS_APP_NAME}/RelWithDebInfo")

SET(COPY_BASILISK_MOD_DLL_DEBUG_TO_TEST ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-mod/Debug/basilisk-mod.dll" "${CMAKE_SOURCE_DIR}/build/basilisk-test-runner/Debug/")
SET(COPY_BASILISK_MOD_DLL_RELEASE_TO_TEST ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-mod/Release/basilisk-mod.dll" "${CMAKE_SOURCE_DIR}/build/basilisk-test-runner/Release")
SET(COPY_BASILISK_MOD_DLL_RELEASE_DEBUG_TO_TEST ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-mod/RelWithDebInfo/basilisk-mod.dll" "${CMAKE_SOURCE_DIR}/build/basilisk-test-runner/RelWithDebInfo")

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:Debug>:${COPY_BASILISK_MOD_DLL_DEBUG}>" COMMENT "POST_BUILD: Copying basilisk-mod.dll/"
    COMMAND "$<$<CONFIG:Debug>:${COPY_BASILISK_MOD_DLL_DEBUG_TO_TEST}>" COMMENT "POST_BUILD: Copying basilisk-mod.dll/ to test"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS Debug
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:Release>:${COPY_BASILISK_MOD_DLL_RELEASE}>" COMMENT "POST_BUILD: Copying basilisk-mod.dll/"
    COMMAND "$<$<CONFIG:Release>:${COPY_BASILISK_MOD_DLL_RELEASE_TO_TEST}>" COMMENT "POST_BUILD: Copying basilisk-mod.dll/ to test"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS Release
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_BASILISK_MOD_DLL_RELEASE_DEBUG}>" COMMENT "POST_BUILD: Copying basilisk-mod.dll/"
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_BASILISK_MOD_DLL_RELEASE_DEBUG_TO_TEST}>" COMMENT "POST_BUILD: Copying basilisk-mod.dll/ to test"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS RelWithDebInfo
)