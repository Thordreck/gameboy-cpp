add_library(mealybug_tests_core)
target_compile_features(mealybug_tests_core PUBLIC cxx_std_23)
add_library(mealybug_tests::core ALIAS mealybug_tests_core)

set_target_properties(mealybug_tests_core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(mealybug_tests_core PUBLIC emulator::core emulator::gameboy doctest stb_image)
target_sources(mealybug_tests_core PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/mealybug_tests_core.cppm
)

add_executable(mealybug_tests)
target_compile_features(mealybug_tests PUBLIC cxx_std_23)
target_link_libraries(mealybug_tests PRIVATE mealybug_tests::core doctest::doctest_with_main)
set_target_properties(mealybug_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(mealybug_tests WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(mealybug_tests
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/mealybug_suite.test.cpp
)
