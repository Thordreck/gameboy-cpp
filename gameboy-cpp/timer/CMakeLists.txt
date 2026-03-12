
add_library(timer)
target_compile_features(timer PUBLIC cxx_std_23)

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
		${CMAKE_CURRENT_SOURCE_DIR}/timer_tma.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/timer_system.cppm
)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
	add_executable(timer_tests)
	target_compile_features(timer_tests PUBLIC cxx_std_23)
	target_link_libraries(timer_tests PRIVATE timer doctest::doctest_with_main)
	set_target_properties(timer_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
	doctest_discover_tests(timer_tests)

	target_sources(timer_tests 
		PRIVATE 
			${CMAKE_CURRENT_SOURCE_DIR}/timer_system.test.cpp
	)

endif()
