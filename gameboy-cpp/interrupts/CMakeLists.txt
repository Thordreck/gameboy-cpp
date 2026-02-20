
add_library(interrupts)
target_compile_features(interrupts PUBLIC cxx_std_23)
add_std_ifc_to_target(interrupts)

set_target_properties(interrupts PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(interrupts)
target_sources(interrupts PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts.cppm
)
