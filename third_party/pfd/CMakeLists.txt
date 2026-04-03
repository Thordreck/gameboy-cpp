
add_library(pfd)
target_compile_features(pfd PUBLIC cxx_std_23)
target_include_directories(pfd PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(pfd PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_sources(pfd PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/portable_file_dialogs.cppm
)
