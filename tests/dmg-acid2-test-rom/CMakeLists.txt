add_executable(dmg_acid2_test)
target_compile_features(dmg_acid2_test PUBLIC cxx_std_23)
target_link_libraries(dmg_acid2_test PRIVATE engine tests::core doctest::doctest_with_main)
set_target_properties(dmg_acid2_test PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_compile_warnings(dmg_acid2_test)
doctest_discover_tests(dmg_acid2_test WORKING_DIRECTORY "${CMAKE_CURRENT_SOURCE_DIR}")

target_sources(dmg_acid2_test
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/dmg_acid2.test.cpp
)