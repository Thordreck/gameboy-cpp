add_library(scribbl_tests_core)
target_compile_features(scribbl_tests_core PUBLIC cxx_std_23)
add_library(scribbl_tests::core ALIAS scribbl_tests_core)

set_target_properties(scribbl_tests_core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(scribbl_tests_core PUBLIC emulator::core emulator::gameboy doctest stb_image)
target_sources(scribbl_tests_core PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/scribbl_tests_core.cppm
)

add_executable(scribbl_tests)
target_compile_features(scribbl_tests PUBLIC cxx_std_23)
target_link_libraries(scribbl_tests PRIVATE scribbl_tests::core doctest::doctest_with_main)
set_target_properties(scribbl_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(scribbl_tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(scribbl_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/statcount.test.cpp
)
