project(ray_marching_lib)

add_library(ray_marching_lib INTERFACE)
target_include_directories(ray_marching_lib INTERFACE ${CMAKE_SOURCE_DIR}/include)
