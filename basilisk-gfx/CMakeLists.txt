cmake_minimum_required(VERSION 3.23.2)
project(basilisk-gfx)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE GENERATED_FILES "generated/*.*")

file(GLOB SOURCE_FILES "src/*.c")
file(GLOB XML_FILES "../xml/*.*")

file(GLOB TYPE_FILES "src/types/*.*")

file(GLOB PREFAB_FILES "src/types/prefab/*.*")
file(GLOB PRIMITIVE_FILES "src/types/primitive/*.*")
file(GLOB LIGHT_FILES "src/types/light/*.*")
file(GLOB SPAWNER_FILES "src/types/spawner/*.*")
file(GLOB TILE_FILES "src/types/tile/*.*")
file(GLOB FOLIAGE_FILES "src/types/foliage/*.*")

file(GLOB ATLAS_FILES "src/objects/atlas/*.*")
file(GLOB FONT_FILES "src/objects/font/*.*")
file(GLOB TEXTURE_FILES "src/objects/texture/*.*")

file(GLOB UI_FILES "src/ui/*.*")
file(GLOB UI_MAIN_FILES "src/ui/main/*.*")

file(GLOB_RECURSE GLSL_FILES "shaders/*.glsl")
file(GLOB_RECURSE FRAG_FILES "shaders/*.frag")
file(GLOB_RECURSE VERT_FILES "shaders/*.vert")
file(GLOB_RECURSE COMP_FILES "shaders/*.comp")
file(GLOB_RECURSE RGEN_FILES "shaders/*.rgen")
file(GLOB_RECURSE RMISS_FILES "shaders/*.rmiss")

file(GLOB ENTITY_FILES "src/spawners/entity/*.*")
file(GLOB ITEM_FILES "src/spawners/item/*.*")
file(GLOB SPAWNERS_FILES "src/spawners/*.*")

file(GLOB_RECURSE HEADER_FILES "include/*.h")
file(GLOB PROJECT_FILES "./*" LIST_DIRECTORIES false)

add_library(${PROJECT_NAME} SHARED
    bsgfx_contracts.h
    ${XML_FILES}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${GENERATED_FILES}

    ${ATLAS_FILES}
    ${FONT_FILES}
    ${TEXTURE_FILES}

    ${UI_FILES}
    ${UI_MAIN_FILES}

    ${GLSL_FILES}
    ${FRAG_FILES}
    ${VERT_FILES}
    ${COMP_FILES}
    ${RGEN_FILES}
    ${RMISS_FILES}

    ${TYPE_FILES}
    ${PREFAB_FILES}
    ${PRIMITIVE_FILES}
    ${LIGHT_FILES}
    ${SPAWNER_FILES}
    ${TILE_FILES}
    ${FOLIAGE_FILES}
)

source_group("Views" FILES ${VIEWS})
source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Generated Files" FILES ${GENERATED_FILES})
source_group("XML Files" FILES ${XML_FILES})

source_group("Source Files/Objects/Atlas" FILES ${ATLAS_FILES})
source_group("Source Files/Objects/Clothing" FILES ${CLOTHING_FILES})
source_group("Source Files/Objects/Font" FILES ${FONT_FILES})
source_group("Source Files/Objects/Texture" FILES ${TEXTURE_FILES})

source_group("Source Files/Types/Prefab" FILES ${PREFAB_FILES})
source_group("Source Files/Types/Primitive" FILES ${PRIMITIVE_FILES})
source_group("Source Files/Types/Spawner" FILES ${SPAWNER_FILES})
source_group("Source Files/Types/Tile" FILES ${TILE_FILES})
source_group("Source Files/Types/Light" FILES ${LIGHT_FILES})
source_group("Source Files/Types/Foliage" FILES ${FOLIAGE_FILES})

source_group("Source Files/Spawners/Entity" FILES ${ENTITY_FILES})
source_group("Source Files/Spawners/Item" FILES ${ITEM_FILES})
source_group("Source Files/Spawners" FILES ${SPAWNERS_FILES})

source_group("Source Files/UI" FILES ${UI_FILES})
source_group("Source Files/UI/Main" FILES ${UI_MAIN_FILES})

source_group("Shaders" FILES ${GLSL_FILES})
source_group("Shaders/vert" FILES ${VERT_FILES})
source_group("Shaders/frag" FILES ${FRAG_FILES})
source_group("Shaders/comp" FILES ${COMP_FILES})
source_group("Shaders/rgen" FILES ${RGEN_FILES})
source_group("Shaders/rmiss" FILES ${RMISS_FILES})

source_group("Source Files/Types" FILES ${TYPE_FILES})

source_group("Headers" FILES ${HEADER_FILES})
source_group("Project Files" FILES ${PROJECT_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/basilisk-core/generated
    ${CMAKE_SOURCE_DIR}/basilisk-core/include
    ${CMAKE_SOURCE_DIR}/basilisk-steam/include
    ${CMAKE_SOURCE_DIR}/external
    ${CMAKE_SOURCE_DIR}/external/GFSDK_Aftermath_Lib
    ${CMAKE_SOURCE_DIR}/external/vulkan
    generated/
    include/
    src/
    ${CMAKE_SOURCE_DIR}/include/objects/
    levels/
)

target_link_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    basilisk-core
    basilisk-steam
    vulkan-1 
    steam_api64 
    dwmapi
    GFSDK_Aftermath_Lib.x64
)

SET(COPY_BASILISK_GFX_DLL_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-gfx/Debug/basilisk-gfx.dll" "${CMAKE_SOURCE_DIR}/build/${BS_APP_NAME}/Debug/")
SET(COPY_BASILISK_GFX_DLL_RELEASE ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-gfx/Release/basilisk-gfx.dll" "${CMAKE_SOURCE_DIR}/build/${BS_APP_NAME}/Release")
SET(COPY_BASILISK_GFX_DLL_RELEASE_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-gfx/RelWithDebInfo/basilisk-gfx.dll" "${CMAKE_SOURCE_DIR}/build/${BS_APP_NAME}/RelWithDebInfo")

SET(COPY_BASILISK_GFX_DLL_DEBUG_TO_TEST ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-gfx/Debug/basilisk-gfx.dll" "${CMAKE_SOURCE_DIR}/build/basilisk-test-runner/Debug/")
SET(COPY_BASILISK_GFX_DLL_RELEASE_TO_TEST ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-gfx/Release/basilisk-gfx.dll" "${CMAKE_SOURCE_DIR}/build/basilisk-test-runner/Release")
SET(COPY_BASILISK_GFX_DLL_RELEASE_DEBUG_TO_TEST ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/build/basilisk-gfx/RelWithDebInfo/basilisk-gfx.dll" "${CMAKE_SOURCE_DIR}/build/basilisk-test-runner/RelWithDebInfo")

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:Debug>:${COPY_BASILISK_GFX_DLL_DEBUG}>" COMMENT "POST_BUILD: Copying basilisk-gfx.dll/"
    COMMAND "$<$<CONFIG:Debug>:${COPY_BASILISK_GFX_DLL_DEBUG_TO_TEST}>" COMMENT "POST_BUILD: Copying basilisk-gfx.dll/ to test"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS Debug
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:Release>:${COPY_BASILISK_GFX_DLL_RELEASE}>" COMMENT "POST_BUILD: Copying basilisk-gfx.dll/"
    COMMAND "$<$<CONFIG:Release>:${COPY_BASILISK_GFX_DLL_RELEASE_TO_TEST}>" COMMENT "POST_BUILD: Copying basilisk-gfx.dll/ to test"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS Release
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_BASILISK_GFX_DLL_RELEASE_DEBUG}>" COMMENT "POST_BUILD: Copying basilisk-gfx.dll/"
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_BASILISK_GFX_DLL_RELEASE_DEBUG_TO_TEST}>" COMMENT "POST_BUILD: Copying basilisk-gfx.dll/ to test"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS RelWithDebInfo
)