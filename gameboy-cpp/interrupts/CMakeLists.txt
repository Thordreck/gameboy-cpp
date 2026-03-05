
add_library(interrupts)
target_compile_features(interrupts PUBLIC cxx_std_23)
add_std_ifc_to_target(interrupts)

set_target_properties(interrupts PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(interrupts cpu memory utilities)
target_sources(interrupts PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts_common.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts_request.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts_enable.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts_dispatch.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts_service.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/interrupts_processor.cppm
)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
	add_library(interrupts_test_utilities)
	target_compile_features(interrupts_test_utilities PUBLIC cxx_std_23)
	add_std_ifc_to_target(interrupts_test_utilities)

	set_target_properties(interrupts_test_utilities PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
	target_link_libraries(interrupts_test_utilities interrupts)
	target_sources(interrupts_test_utilities PUBLIC 
		FILE_SET 
			CXX_MODULES 
		FILES 
			${CMAKE_CURRENT_SOURCE_DIR}/interrupts_tests.cppm
	)

	add_executable(interrupts_tests)
	target_compile_features(interrupts_tests PUBLIC cxx_std_23)
	target_link_libraries(interrupts_tests PRIVATE interrupts cpu memory interrupts_test_utilities doctest::doctest_with_main)
	set_target_properties(interrupts_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
	doctest_discover_tests(interrupts_tests)
	add_std_ifc_to_target(interrupts_tests)

	target_sources(interrupts_tests 
		PRIVATE 
			${CMAKE_CURRENT_SOURCE_DIR}/interrupts_enable.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/interrupts_request.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/interrupts_dispatch.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/interrupts_service.test.cpp
	)

endif()
