add_library(serial)
target_compile_features(serial PUBLIC cxx_std_23)

set_target_properties(serial PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(serial memory interrupts)
target_sources(serial PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/serial.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/serial_common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/serial_memory.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/serial_link.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/serial_dummy.cppm
)
