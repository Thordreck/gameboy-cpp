
add_library(graphics)
target_compile_features(graphics PUBLIC cxx_std_23)
add_std_ifc_to_target(graphics)

set_target_properties(graphics PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(graphics)
target_sources(graphics PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/graphics.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/graphics_common.cppm
)
