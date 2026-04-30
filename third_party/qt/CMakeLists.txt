include(cmake/qt_setup.cmake)

add_library(qt)
target_compile_features(qt PUBLIC cxx_std_23)
set_target_properties(qt PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(qt PRIVATE Qt6::Core Qt6::Qml Qt6::Quick)

target_sources(qt PUBLIC
        FILE_SET
        CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/qt.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_gui.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/qt_qml.cppm
)
