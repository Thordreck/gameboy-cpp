add_library(mooneye_suite_core)
target_compile_features(mooneye_suite_core PUBLIC cxx_std_23)
add_library(mooneye_suite::core ALIAS mooneye_suite_core)

set_target_properties(mooneye_suite_core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(mooneye_suite_core PUBLIC emulator::core doctest)
target_sources(mooneye_suite_core PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/mooneye_tests_core.cppm
)

add_executable(mooneye_suite_tests)
target_compile_features(mooneye_suite_tests PUBLIC cxx_std_23)
target_link_libraries(mooneye_suite_tests PRIVATE mooneye_suite::core doctest::doctest_with_main)
set_target_properties(mooneye_suite_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(mooneye_suite_tests)

target_sources(mooneye_suite_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/acceptance.test.cpp
        ${CMAKE_CURRENT_SOURCE_DIR}/emulator_only.test.cpp
)

# Copy ROMs to build directory
file(GLOB_RECURSE TEST_ROMS "${CMAKE_CURRENT_SOURCE_DIR}/roms/*.gb")

foreach(ROM ${TEST_ROMS})
    configure_file(${ROM}
            ${CMAKE_CURRENT_BINARY_DIR}
            COPYONLY)
endforeach()
