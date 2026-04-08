
option(SYSTEM_SDL "Use an sdl3 distribution installed in the system" OFF)

if(SYSTEM_SDL)
    find_package(SDL3 REQUIRED CONFIG REQUIRED COMPONENTS SDL3-shared)
else()
    add_subdirectory(upstream EXCLUDE_FROM_ALL)
endif()

add_library(sdl)
target_compile_features(sdl PUBLIC cxx_std_23)
set_target_properties(sdl PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(sdl PUBLIC SDL3::SDL3)

target_sources(sdl PUBLIC
        FILE_SET
        CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_internal.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_init.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_timer.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_window.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_render.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_surface.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_texture.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_rect.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_events.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_scancode.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/sdl_keyboard.cppm
)