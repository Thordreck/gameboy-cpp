
# TODO: support for more backend pairs
set(IMGUI_PLATFORM_BACKEND "SDL3" CACHE STRING "imgui platform backend")
set_property(CACHE IMGUI_PLATFORM_BACKEND PROPERTY STRINGS SDL3)

set(IMGUI_RENDERER_BACKEND "SDL3_RENDERER" CACHE STRING "imgui renderer backend")
set_property(CACHE IMGUI_RENDERER_BACKEND PROPERTY STRINGS SDL3_RENDERER)

# Core
add_library(imgui-core STATIC
	${CMAKE_CURRENT_SOURCE_DIR}/upstream/imgui.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/upstream/imgui_draw.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/upstream/imgui_tables.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/upstream/imgui_widgets.cpp
	${CMAKE_CURRENT_SOURCE_DIR}/upstream/imgui_demo.cpp)

target_include_directories(imgui-core PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/upstream)
target_compile_definitions(imgui-core PUBLIC
		IMGUI_USER_CONFIG="${CMAKE_CURRENT_SOURCE_DIR}/imgui_config.h"
		IMGUI_DISABLE_DEMO_WINDOWS
		IMGUI_DISABLE_OBSOLETE_FUNCTIONS)

add_library(imgui::core ALIAS imgui-core)

# Renderer backend
add_library(imgui-renderer STATIC)
target_include_directories(imgui-renderer PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/upstream/backends)
target_link_libraries(imgui-renderer PUBLIC imgui-core)
add_library(imgui::renderer ALIAS imgui-renderer)

if(IMGUI_RENDERER_BACKEND STREQUAL "SDL3_RENDERER")
	target_sources(imgui-renderer PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/upstream/backends/imgui_impl_sdlrenderer3.cpp)
	target_link_libraries(imgui-renderer PUBLIC sdl)
endif()

# Platform backend
add_library(imgui-platform STATIC)
target_include_directories(imgui-platform PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/upstream/backends)
target_link_libraries(imgui-platform PUBLIC imgui-core)
add_library(imgui::platform ALIAS imgui-platform)

if(IMGUI_PLATFORM_BACKEND STREQUAL "SDL3")
	target_sources(imgui-platform PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/upstream/backends/imgui_impl_sdl3.cpp)
	target_link_libraries(imgui-platform PUBLIC sdl)
endif()

add_library(imgui)
target_compile_features(imgui PUBLIC cxx_std_23)
set_target_properties(imgui PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(imgui PRIVATE imgui::core imgui::platform imgui::renderer)

target_sources(imgui PUBLIC 
	FILE_SET 
		CXX_MODULES 
	BASE_DIRS
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/imgui.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_common.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_widgets.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_style.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_assert.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_init.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_frame.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_demo.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_sdl3.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/imgui_viewport.cppm
)