
add_library(math)
target_compile_features(math PUBLIC cxx_std_23)
target_compile_warnings(math)
target_link_libraries(math PRIVATE gcem)
set_target_properties(math PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_sources(math PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/math.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/trigonometry.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/complex.cppm
)

if(BUILD_TESTING)
    add_executable(math_tests)
    target_compile_features(math_tests PUBLIC cxx_std_23)
    target_link_libraries(math_tests PRIVATE math doctest::doctest_with_main)
    set_target_properties(math_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
    doctest_discover_tests(math_tests)
    target_compile_warnings(math_tests)

    target_sources(math_tests
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/trigonometry.test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/common.test.cpp
            ${CMAKE_CURRENT_SOURCE_DIR}/complex.test.cpp
    )

endif()
