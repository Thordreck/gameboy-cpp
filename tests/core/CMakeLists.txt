add_library(tests_core)
target_compile_features(tests_core PUBLIC cxx_std_23)
target_compile_warnings(tests_core)
add_library(tests::core ALIAS tests_core)

set_target_properties(tests_core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(tests_core PRIVATE engine stb_image utilities doctest)
target_sources(tests_core PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/core.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/required.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/engine.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/serial.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/graphics.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/memory.cppm
)