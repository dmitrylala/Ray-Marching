project(ray_marching_lib)

add_library(ray_marching_lib STATIC
            ray_marcher.cpp
            utils.cpp
            ${CMAKE_SOURCE_DIR}/external/LiteMath/Image2d.cpp)

target_include_directories(ray_marching_lib PUBLIC 
                            ${CMAKE_SOURCE_DIR}/include
                            ${CMAKE_SOURCE_DIR}/external/LiteMath)
