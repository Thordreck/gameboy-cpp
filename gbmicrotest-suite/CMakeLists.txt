add_library(gbmicro_tests_core)
target_compile_features(gbmicro_tests_core PUBLIC cxx_std_23)
add_library(gbmicro::core ALIAS gbmicro_tests_core)

set_target_properties(gbmicro_tests_core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(gbmicro_tests_core PUBLIC emulator::core emulator::gameboy doctest)
target_sources(gbmicro_tests_core PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/gbmicrotest_core.cppm
)

add_executable(gbmicro_tests)
target_compile_features(gbmicro_tests PUBLIC cxx_std_23)
target_link_libraries(gbmicro_tests PUBLIC gbmicro::core doctest::doctest_with_main)
set_target_properties(gbmicro_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(gbmicro_tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(gbmicro_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/gbmicrotest_suite.test.cpp
)
