add_library(json)
target_compile_features(json PUBLIC cxx_std_23)
target_compile_warnings(json)

set_target_properties(json PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(json PRIVATE utilities)
target_sources(json PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/json.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/types.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/serialization.cppm
)

set(JSON_PROVIDER "Qt" CACHE STRING "Json backend provider")
set_property(CACHE JSON_PROVIDER PROPERTY STRINGS Qt nlohmann)

if(JSON_PROVIDER STREQUAL "Qt")
    target_link_libraries(json PRIVATE Qt6::Core)
    target_sources(json PUBLIC
            FILE_SET
            CXX_MODULES
            FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/json_qt.cppm
    )
elseif(JSON_PROVIDER STREQUAL "nlohmann")
    target_link_libraries(json PRIVATE nlohmann_json_modules)
    target_sources(json PUBLIC
            FILE_SET
            CXX_MODULES
            FILES
            ${CMAKE_CURRENT_SOURCE_DIR}/json_nlohmann.cppm)
else()
    message(FATAL_ERROR "No valid json provider set")
endif()

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    add_executable(json_tests)
    target_compile_features(json_tests PUBLIC cxx_std_23)
    target_compile_warnings(json_tests)
    target_link_libraries(json_tests PRIVATE json utilities doctest::doctest_with_main)
    target_sources(json_tests PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/json.test.cpp)

    if(JSON_PROVIDER STREQUAL "Qt")
        deploy_qt_libraries(json_tests)
    endif()

    doctest_discover_tests(json_tests)
endif()


