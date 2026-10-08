
add_library(engine)
target_compile_features(engine PUBLIC cxx_std_23)
target_compile_warnings(engine)

set_target_properties(engine PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(engine PRIVATE cpu opcodes mbc memory audio joypad serial graphics cartridge profiling)
target_sources(engine PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/module.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/base.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/engine.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/factory.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/scheduler.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/cpu.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/memory.cppm
)
