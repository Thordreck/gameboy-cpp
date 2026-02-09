
add_library(cpu)
target_compile_features(cpu PUBLIC cxx_std_23)
add_std_ifc_to_target(cpu)

set_target_properties(cpu PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(cpu utilities)
target_sources(cpu PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/cpu.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/registers.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions.cppm
)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
	add_executable(cpu_tests)
	target_compile_features(cpu_tests PUBLIC cxx_std_23)
	target_link_libraries(cpu_tests PRIVATE cpu doctest::doctest_with_main)
	set_target_properties(cpu_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
	doctest_discover_tests(cpu_tests)
	add_std_ifc_to_target(cpu_tests)

	target_sources(cpu_tests 
		PRIVATE 
			${CMAKE_CURRENT_SOURCE_DIR}/registers.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions.test.cpp
	)

endif()
