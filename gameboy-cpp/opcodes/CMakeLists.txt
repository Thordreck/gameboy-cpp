
add_library(opcodes)
target_compile_features(opcodes PUBLIC cxx_std_23)
add_std_ifc_to_target(opcodes)

set_target_properties(opcodes PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(opcodes cpu utilities)
target_sources(opcodes PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/instructions.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_table.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_common.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_arithmetic.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_jump.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_load.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_stack.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_subroutine.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_misc.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_interrupt.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_bit.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_carry.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_fetch.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_decode.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_execute.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_pipeline.cppm
)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
	add_executable(opcodes_tests)
	target_compile_features(opcodes_tests PUBLIC cxx_std_23)
	target_link_libraries(opcodes_tests PRIVATE opcodes doctest::doctest_with_main)
	set_target_properties(opcodes_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
	doctest_discover_tests(opcodes_tests)
	add_std_ifc_to_target(opcodes_tests)

	target_sources(opcodes_tests 
		PRIVATE 
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_arithmetic.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_jump.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_load.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_table.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_stack.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_subroutine.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_misc.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_bit.test.cpp
			${CMAKE_CURRENT_SOURCE_DIR}/instructions_carry.test.cpp
	)

endif()
