add_library(not_chciken_tests_core)
target_compile_features(not_chciken_tests_core PUBLIC cxx_std_23)
add_library(not_chciken::core ALIAS not_chciken_tests_core)

set_target_properties(not_chciken_tests_core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(not_chciken_tests_core PRIVATE emulator::core emulator::gameboy doctest plot dsp sdl svg)
target_sources(not_chciken_tests_core PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/not_chciken_tests_core.cppm
)

add_executable(not_chciken_tests)
target_compile_features(not_chciken_tests PUBLIC cxx_std_23)
target_link_libraries(not_chciken_tests PRIVATE not_chciken::core doctest::doctest_with_main)
set_target_properties(not_chciken_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(not_chciken_tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(not_chciken_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/square.test.cpp
)
