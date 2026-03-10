
# TODO: support for other backends instead of vulkan + glfw combo
find_package(Vulkan 1.4.334 REQUIRED)
find_package(glfw3 REQUIRED)
find_package(Freetype)

add_library(imgui)
target_compile_features(imgui PUBLIC cxx_std_23)
set_target_properties(imgui PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)

target_link_libraries(imgui 
	PUBLIC 
		$<$<TARGET_EXISTS:Freetype::Freetype>:Freetype::Freetype>
		glfw
		Vulkan::Vulkan)

target_compile_definitions(imgui
	PUBLIC
		$<$<TARGET_EXISTS:Freetype::Freetype>:IMGUI_ENABLE_FREETYPE>
		IMGUI_DISABLE_OBSOLETE_FUNCTIONS
		VULKAN_HPP_CXX_MODULE_EXPERIMENTAL_WARNING
		GLFW_INCLUDE_VULKAN
		VK_ENABLE_BETA_EXTENSIONS
)

add_std_ifc_to_target(imgui)

target_include_directories(imgui 
	PUBLIC 
		$<TARGET_PROPERTY:glfw,INTERFACE_INCLUDE_DIRECTORIES>
		${CMAKE_CURRENT_SOURCE_DIR}/src 
		${CMAKE_CURRENT_SOURCE_DIR}/src/backends
		$<$<TARGET_EXISTS:Freetype::Freetype>:${CMAKE_CURRENT_SOURCE_DIR}/src/misc/freetype>
)

target_sources(imgui PUBLIC 
	FILE_SET 
		CXX_MODULES 
	BASE_DIRS
		${Vulkan_INCLUDE_DIR}
		${CMAKE_CURRENT_SOURCE_DIR}
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/src/imgui.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_internal.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_vulkan.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_glfw.cppm
		${Vulkan_INCLUDE_DIR}/vulkan/vulkan.cppm
		${Vulkan_INCLUDE_DIR}/vulkan/vulkan_video.cppm
		$<$<TARGET_EXISTS:Freetype::Freetype>:${CMAKE_CURRENT_SOURCE_DIR}/src/misc/freetype/imgui_freetype.cppm>
)

target_sources(imgui 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/src/imgui.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_draw.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_tables.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_widgets.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/imgui_demo.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_vulkan.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/src/backends/imgui_impl_glfw.cpp
		$<$<TARGET_EXISTS:Freetype::Freetype>:${CMAKE_CURRENT_SOURCE_DIR}/src/misc/freetype/imgui_freetype.cpp>
)

