
add_library(memory)
target_compile_features(memory PUBLIC cxx_std_23)

set_target_properties(memory PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(memory utilities)
target_sources(memory PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/memory.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/memory_common.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/memory_bus.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/memory_map.cppm
)

