
add_library(blargg_tests_utilities)
target_compile_features(blargg_tests_utilities PUBLIC cxx_std_23)
add_std_ifc_to_target(blargg_tests_utilities)

set_target_properties(blargg_tests_utilities PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(blargg_tests_utilities PUBLIC emulator memory cpu opcodes timer interrupts doctest)
target_sources(blargg_tests_utilities PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/blargg_tests.cppm
)

add_executable(blargg_tests)
target_compile_features(blargg_tests PUBLIC cxx_std_23)
target_link_libraries(blargg_tests PRIVATE blargg_tests_utilities doctest::doctest_with_main)
set_target_properties(blargg_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(blargg_tests)
add_std_ifc_to_target(blargg_tests)

target_sources(blargg_tests 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/cpu_instructions.test.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/instructions_timing.test.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/interrupt_time.test.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/memory_timing.test.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/memory_timing_2.test.cpp
		${CMAKE_CURRENT_SOURCE_DIR}/halt_bug.test.cpp
)

# Copy ROMs to build directory
file(GLOB_RECURSE TEST_ROMS "${CMAKE_CURRENT_SOURCE_DIR}/roms/*.gb")

foreach(ROM ${TEST_ROMS})
    configure_file(${ROM}
                   ${CMAKE_CURRENT_BINARY_DIR}
                   COPYONLY)
endforeach()
