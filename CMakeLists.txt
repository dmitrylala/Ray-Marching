cmake_minimum_required(VERSION 3.16)
project(ray_marching)

set (CMAKE_CXX_STANDARD 17)
# option(USE_VULKAN "Enable GPU implementation via Vulkan" OFF)

find_package(OpenMP)

message(STATUS "Cmake binary dir: " ${CMAKE_BINARY_DIR})
message(STATUS "C++ standard: " ${CMAKE_CXX_STANDARD})

add_subdirectory(lib/)
add_subdirectory(bin/)
# add_subdirectory(test/unit)
