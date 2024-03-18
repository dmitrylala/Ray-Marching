project(ray_marching_app)

set(EXE_SOURCES
	main.cpp
)

if (NOT USE_VULKAN)
	add_executable(raymarching ${EXE_SOURCES})

	# set_target_properties(testapp PROPERTIES VS_DEBUGGER_WORKING_DIRECTORY "${CMAKE_SOURCE_DIR}")
	set_target_properties(raymarching PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

	# target_include_directories(raymarching PUBLIC ${CMAKE_SOURCE_DIR}/external/LiteMath)
	target_link_libraries(raymarching LINK_PUBLIC 
							ray_marching_lib
							OpenMP::OpenMP_CXX)						
endif()

# find_package(stb REQUIRED)

# add_executable(raymarching ${EXE_SOURCES})

# set_target_properties(raymarching PROPERTIES RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

# target_include_directories(raymarching PUBLIC ${CMAKE_SOURCE_DIR}/include)
# target_link_libraries(raymarching stb::stb)

