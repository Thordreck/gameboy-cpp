
add_library(gcem)
target_compile_warnings(gcem)
target_compile_features(gcem PUBLIC cxx_std_23)
set_target_properties(gcem PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_include_directories(gcem SYSTEM PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/upstream/include)
add_third_party_module(gcem https://github.com/kthohr/gcem ${CMAKE_CURRENT_SOURCE_DIR}/upstream/LICENSE)

target_sources(gcem PUBLIC
        FILE_SET
        CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/gcem.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/trigonometry.cppm
)
