cmake_minimum_required(VERSION 3.23.2)
project(basilisk)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB SOURCE_FILES "src/*.c")

file(GLOB_RECURSE HEADER_FILES "include/*.h")
file(GLOB PROJECT_FILES "./*" LIST_DIRECTORIES false)

file(GLOB XML_FILES "../xml/*.*")

add_executable(${PROJECT_NAME}
    ${XML_FILES}
    ${SOURCE_FILES}
    ${HEADER_FILES}

    ${VS_SHADER_FILES}
    ${FS_SHADER_FILES}
)

set_property(TARGET ${PROJECT_NAME}
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/$<CONFIG>"
)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Headers" FILES ${HEADER_FILES})
source_group("Project Files" FILES ${PROJECT_FILES})
source_group("XML Files" FILES ${XML_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_SOURCE_DIR}/basilisk-core/include
    ${CMAKE_SOURCE_DIR}/basilisk-gfx/include
    ${CMAKE_SOURCE_DIR}/basilisk-mod/include

    ${CMAKE_SOURCE_DIR}/basilisk-core/generated
    ${CMAKE_SOURCE_DIR}/basilisk-gfx/generated
    ${CMAKE_SOURCE_DIR}/basilisk-mod/generated

    ${CMAKE_SOURCE_DIR}/external
    include/
    src/
)

add_dependencies(${PROJECT_NAME}
    basilisk-core
    basilisk-gfx
    basilisk-mod
)

target_link_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/external/
)

target_link_libraries(${PROJECT_NAME} PRIVATE
    basilisk-core
    basilisk-gfx
    basilisk-mod
    vulkan-1 
    dwmapi
)
