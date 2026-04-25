add_library(cartridge)
target_compile_features(cartridge PUBLIC cxx_std_23)

set_target_properties(cartridge PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(cartridge utilities)
target_sources(cartridge PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/cartridge.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/cartridge_common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/cartridge_rom.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/cartridge_header.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/cartridge_parser.cppm
)
