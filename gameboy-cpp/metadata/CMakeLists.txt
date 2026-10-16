add_library(metadata)
target_compile_features(metadata PUBLIC cxx_std_23)
target_compile_warnings(metadata)

set_target_properties(metadata PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(metadata PRIVATE json)

get_git_commit_hash(GIT_HASH ERROR_IS_FATAL)
configure_file(
        ${CMAKE_CURRENT_SOURCE_DIR}/project_metadata.cppm.in
        ${CMAKE_BINARY_DIR}/generated/project_metadata.cppm)

target_sources(metadata PUBLIC
        FILE_SET
        CXX_MODULES
        BASE_DIRS
        ${CMAKE_BINARY_DIR}
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/metadata.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/project.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/third_party.cppm
        ${CMAKE_BINARY_DIR}/generated/project_metadata.cppm
)
link_third_party_module(metadata)

