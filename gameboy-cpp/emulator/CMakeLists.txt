
add_library(emulator)
target_compile_features(emulator PUBLIC cxx_std_23)

set_target_properties(emulator PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator cpu timer memory opcodes interrupts utilities)
target_sources(emulator PUBLIC 
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/emulator.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/cpu_runner.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/memory_map.cppm
)

# TODO: testing imgui installation
add_executable(gameboy-cpp)
target_compile_features(gameboy-cpp PUBLIC cxx_std_23)
target_link_libraries(gameboy-cpp PRIVATE imgui)

target_sources(gameboy-cpp 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
	add_executable(emulator_tests)
	target_compile_features(emulator_tests PUBLIC cxx_std_23)
	target_link_libraries(emulator_tests PRIVATE emulator doctest::doctest_with_main)
	set_target_properties(emulator_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
	doctest_discover_tests(emulator_tests)

	target_sources(emulator_tests 
		PRIVATE 
			${CMAKE_CURRENT_SOURCE_DIR}/cpu_runner.test.cpp
	)

endif()
