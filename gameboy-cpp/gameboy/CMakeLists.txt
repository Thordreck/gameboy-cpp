add_executable(gameboy-cpp)
target_compile_features(gameboy-cpp PUBLIC cxx_std_23)
target_compile_warnings(gameboy-cpp)
target_link_libraries(gameboy-cpp PRIVATE emulator profiling)

target_sources(gameboy-cpp
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)

# Deployment
add_custom_command(TARGET gameboy-cpp POST_BUILD
        COMMAND ${CMAKE_COMMAND} -E copy -t
        $<TARGET_FILE_DIR:gameboy-cpp>
        $<TARGET_RUNTIME_DLLS:gameboy-cpp>
        COMMAND_EXPAND_LISTS
)

if(UI_BACKEND STREQUAL "SDL")
    add_custom_command(TARGET gameboy-cpp POST_BUILD
            COMMAND ${CMAKE_COMMAND} -E copy_directory
            ${CMAKE_SOURCE_DIR}/resources
            $<TARGET_FILE_DIR:gameboy-cpp>/assets
    )
endif()

if (WIN32 AND (UI_BACKEND STREQUAL "Qt" OR JOYPAD_BACKEND STREQUAL "Qt" OR AUDIO_BACKEND STREQUAL "Qt"))
    add_custom_command(TARGET gameboy-cpp POST_BUILD
            COMMAND ${Qt6_DIR}/../../../bin/windeployqt.exe
            --qmldir ${CMAKE_SOURCE_DIR}/gameboy-cpp/emulator
            --no-translations
            $<TARGET_FILE:gameboy-cpp>
    )
endif()
