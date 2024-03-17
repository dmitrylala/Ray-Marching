project(ray_marching_app)

set(EXE_SOURCES
	main.cpp
)

# find_package(stb REQUIRED)

add_executable(raymarching ${EXE_SOURCES})

set_target_properties(raymarching PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

target_include_directories(raymarching PUBLIC ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(raymarching stb::stb)

