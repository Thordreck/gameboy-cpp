

add_library(utilities)
target_compile_features(utilities PUBLIC cxx_std_23)

set_target_properties(utilities PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_sources(utilities PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/utilities.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/arithmetic.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/memory.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/bits.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/panic.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/assert.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/fixed_size_queue.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/fixed_capacity_vector.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/io.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/clock.cppm
)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
	add_executable(utilities_tests)
	target_compile_features(utilities_tests PUBLIC cxx_std_23)
	target_link_libraries(utilities_tests PRIVATE utilities doctest::doctest_with_main)
	set_target_properties(utilities_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
	doctest_discover_tests(utilities_tests)

	target_sources(utilities_tests
		PRIVATE 
			${CMAKE_CURRENT_SOURCE_DIR}/arithmetic.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/fixed_size_queue.test.cpp
	)

endif()
