cmake_minimum_required(VERSION 3.16)
project(ray_marching)

set (CMAKE_CXX_STANDARD 17)
option(USE_VULKAN "Enable GPU implementation via Vulkan" OFF)

find_package(OpenMP)

message(STATUS "Cmake binary dir: " ${CMAKE_BINARY_DIR})
message(STATUS "C++ standard: " ${CMAKE_CXX_STANDARD})

# if (NOT USE_VULKAN)
#   include_directories("external/LiteMath")
#   add_executable(testapp main.cpp
#                  example_tracer/example_tracer.cpp
#                  external/LiteMath/Image2d.cpp)

#   set_target_properties(testapp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")

#   target_link_libraries(testapp LINK_PUBLIC OpenMP::OpenMP_CXX)
# endif()

add_subdirectory(lib/)
add_subdirectory(bin/)
# add_subdirectory(test/unit)
