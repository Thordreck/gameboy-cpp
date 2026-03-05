
add_library(timer)
target_compile_features(timer PUBLIC cxx_std_23)
add_std_ifc_to_target(timer)

set_target_properties(timer PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(timer memory utilities interrupts)
target_sources(timer PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/timer.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/timer_common.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/timer_div.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/timer_tima.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/timer_tac.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/timer_system.cppm
)
