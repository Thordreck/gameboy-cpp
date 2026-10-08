add_subdirectory(upstream EXCLUDE_FROM_ALL)

add_library(plot)
target_compile_warnings(plot)
target_compile_features(plot PUBLIC cxx_std_23)
set_target_properties(plot PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(plot PRIVATE signalsmith-plot)

target_sources(plot PUBLIC
        FILE_SET
        CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/plot.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/internal.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/figure.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/plot_2d.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/axis.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/tick.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/line.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/svg.cppm
)