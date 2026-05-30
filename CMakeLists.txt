cmake_minimum_required(VERSION 3.23.2)
project(basilisk)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(BS_APP_NAME ${PROJECT_NAME})

add_subdirectory(basilisk-core/)
add_subdirectory(basilisk-gen/)
add_subdirectory(basilisk-gfx/)
add_subdirectory(basilisk-mod/)
add_subdirectory(basilisk-test/)
add_subdirectory(basilisk-test-runner/)