cmake_minimum_required(VERSION 3.23.2)

project(basilisk-gen)

set(CMAKE_C_STANDARD 17)
set(CMAKE_C_STANDARD_REQUIRED ON)

file(GLOB_RECURSE SOURCE_FILES "src/*.c")
file(GLOB_RECURSE HEADER_FILES "include/*.h")
file(GLOB_RECURSE XML_FILES "xml/*.xml")

add_executable(${PROJECT_NAME}
    ${SOURCE_FILES}
    ${HEADER_FILES}
    ${XML_FILES}
)

source_group("Source Files" FILES ${SOURCE_FILES})
source_group("Header Files" FILES ${HEADER_FILES})
source_group("XML Files" FILES ${XML_FILES})

target_include_directories(${PROJECT_NAME} PRIVATE
    include/
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/include
    ${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build
)

target_link_directories(${PROJECT_NAME} PRIVATE 
    ${CMAKE_SOURCE_DIR}/external/
    ${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build/Release
)

target_link_libraries(${PROJECT_NAME} PRIVATE
	libxml2
)

#target_link_libraries(${PROJECT_NAME} PRIVATE
   # steam_api64 
#)

SET(CLEAN_BUILD_DEBUG ${CMAKE_COMMAND} -E rm -r ${CMAKE_CURRENT_BINARY_DIR}/Debug)
SET(CLEAN_BUILD_RELEASE ${CMAKE_COMMAND} -E rm -r ${CMAKE_CURRENT_BINARY_DIR}/Release)
SET(CLEAN_BUILD_RELEASE_DEBUG ${CMAKE_COMMAND} -E rm -r ${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    PRE_BUILD

    COMMAND "$<$<CONFIG:Debug>:${CLEAN_BUILD_DEBUG}>" COMMENT "PRE_BUILD: Cleaning Debug/"

	COMMAND_EXPAND_LISTS
	CONFIGURATIONS Debug
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    PRE_BUILD

    COMMAND "$<$<CONFIG:Release>:${CLEAN_BUILD_RELEASE}>" COMMENT "PRE_BUILD: Cleaning Release/"

	COMMAND_EXPAND_LISTS
	CONFIGURATIONS Release
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    PRE_BUILD

    COMMAND "$<$<CONFIG:RelWithDebInfo>:${CLEAN_BUILD_RELEASE_DEBUG}>" COMMENT "PRE_BUILD: Cleaning RelWithDebInfo/"

	COMMAND_EXPAND_LISTS
	CONFIGURATIONS RelWithDebInfo
)

SET(COPY_LIBXML2_DLL_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build/Release/libxml2.dll" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/Debug/")
SET(COPY_LIBXML2_DLL_RELEASE ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build/Release/libxml2.dll" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/Release")
SET(COPY_LIBXML2_DLL_RELEASE_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build/Release/libxml2.dll" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/RelWithDebInfo")

SET(COPY_ICONV_DLL_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build/Release/iconv-2.dll" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/Debug/")
SET(COPY_ICONV_DLL_RELEASE ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build/Release/iconv-2.dll" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/Release")
SET(COPY_ICONV_DLL_RELEASE_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/external/libxml2-2.15.3/build/Release/iconv-2.dll" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/RelWithDebInfo")

SET(XML_LINK cmd.exe /c mklink /j "\"${CMAKE_CURRENT_BINARY_DIR}/Debug/xml\"" "\"${PROJECT_SOURCE_DIR}/xml\"")

SET(COPY_XML_RELEASE_DEBUG ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/xml" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/RelWithDebInfo/")
SET(COPY_XML_RELEASE ${CMAKE_COMMAND} -E copy_directory "${CMAKE_SOURCE_DIR}/${PROJECT_NAME}/xml" "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/Release/")

SET(COPY_LICENSE_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_BINARY_DIR}/Debug")
SET(COPY_LICENSE_RELEASE${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_BINARY_DIR}/Release")
SET(COPY_LICENSE_RELEASE_DEBUG ${CMAKE_COMMAND} -E copy "${CMAKE_SOURCE_DIR}/LICENSE" "${CMAKE_CURRENT_BINARY_DIR}/RelWithDebInfo")

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:Debug>:${COPY_LIBXML2_DLL_DEBUG}>" COMMENT "POST_BUILD: Copying libxml2.dll/"
    COMMAND "$<$<CONFIG:Debug>:${COPY_ICONV_DLL_DEBUG}>" COMMENT "POST_BUILD: Copying iconv-2.dll/"
    COMMAND "$<$<CONFIG:Debug>:${XML_LINK}>" COMMENT "POST_BUILD: Creating junction for xml/"
    COMMAND "$<$<CONFIG:Debug>:${COPY_LICENSE_DEBUG}>" COMMENT "POST_BUILD: Copying LICENSE/"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS Debug
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:Release>:${COPY_LIBXML2_DLL_RELEASE}>" COMMENT "POST_BUILD: Copying libxml2.dll/"
    COMMAND "$<$<CONFIG:Release>:${COPY_ICONV_DLL_RELEASE}>" COMMENT "POST_BUILD: Copying iconv-2.dll/"
    COMMAND "$<$<CONFIG:Release>:${COPY_XML_RELEASE}>" COMMENT "POST_BUILD: Copying xml/"
    COMMAND "$<$<CONFIG:Release>:${COPY_LICENSE_RELEASE}>" COMMENT "POST_BUILD: Copying LICENSE/"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS Release
)

add_custom_command(
    TARGET ${PROJECT_NAME} 
    POST_BUILD
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_LIBXML2_DLL_RELEASE_DEBUG}>" COMMENT "POST_BUILD: Copying libxml2.dll/"
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_ICONV_DLL_RELEASE_DEBUG}>" COMMENT "POST_BUILD: Copying iconv-2.dll/"
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_XML_RELEASE_DEBUG}>" COMMENT "POST_BUILD: Copying xml/"
    COMMAND "$<$<CONFIG:RelWithDebInfo>:${COPY_LICENSE_RELEASE_DEBUG}>" COMMENT "POST_BUILD: Copying LICENSE/"
	COMMAND_EXPAND_LISTS
    CONFIGURATIONS RelWithDebInfo
)

set_property(TARGET ${PROJECT_NAME}
    PROPERTY VS_DEBUGGER_WORKING_DIRECTORY
    "${CMAKE_SOURCE_DIR}/build/${PROJECT_NAME}/$<CONFIG>"
)
