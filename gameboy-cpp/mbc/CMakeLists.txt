add_library(mbc)
target_compile_features(mbc PUBLIC cxx_std_23)

set_target_properties(mbc PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(mbc)
target_sources(mbc PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/mbc.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/mbc_common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/mbc_only_rom.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/mbc_mbc1.cppm
)

