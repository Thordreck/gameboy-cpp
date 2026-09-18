
set(USE_SYSTEM_PLUTOVG OFF CACHE BOOL "" FORCE)
set(LUNASVG_DISABLE_LOAD_SYSTEM_FONTS OFF CACHE BOOL "" FORCE)
set(LUNASVG_DISABLE_EXTERNAL_RESOURCES OFF CACHE BOOL "" FORCE)
set(LUNASVG_BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)

add_subdirectory(upstream EXCLUDE_FROM_ALL)

add_library(svg)
target_compile_features(svg PUBLIC cxx_std_23)
set_target_properties(svg PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(svg PRIVATE lunasvg::lunasvg)

target_sources(svg PUBLIC
        FILE_SET
        CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/svg.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/document.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/bitmap.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/internal.cppm
)