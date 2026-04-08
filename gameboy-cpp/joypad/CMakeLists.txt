add_library(joypad)
target_compile_features(joypad PUBLIC cxx_std_23)

set_target_properties(joypad PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(joypad memory interrupts utilities profiling)
target_sources(joypad PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/joypad.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/joypad_common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/joypad_memory.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/joypad_core.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/joypad_system.cppm
)
