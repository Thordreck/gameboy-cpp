
add_executable(blargg_cpu_instructions_test)
target_compile_features(blargg_cpu_instructions_test PUBLIC cxx_std_23)
target_link_libraries(blargg_cpu_instructions_test PRIVATE emulator memory cpu opcodes timer interrupts doctest::doctest_with_main)
set_target_properties(blargg_cpu_instructions_test PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(blargg_cpu_instructions_test)
add_std_ifc_to_target(blargg_cpu_instructions_test)

target_sources(blargg_cpu_instructions_test 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/cpu_instructions.test.cpp
)

# Copy ROMs to build directory
file(GLOB TEST_ROMS "${CMAKE_CURRENT_SOURCE_DIR}/roms/*.gb")

foreach(ROM ${TEST_ROMS})
    configure_file(${ROM}
                   ${CMAKE_CURRENT_BINARY_DIR}
                   COPYONLY)
endforeach()
