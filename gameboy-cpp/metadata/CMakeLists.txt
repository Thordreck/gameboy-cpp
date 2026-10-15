add_library(metadata)
target_compile_features(metadata PUBLIC cxx_std_23)
target_compile_warnings(metadata)

set_target_properties(metadata PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(metadata PRIVATE json)
target_sources(metadata PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/metadata.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party.cppm
)
link_third_party_module(metadata)

