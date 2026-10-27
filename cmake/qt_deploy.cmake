function(deploy_qt_libraries target)
    cmake_parse_arguments(PARSE_ARGV 1 arg "" "QML_DIR" "TRANSLATIONS")

    IF(NOT TARGET ${target})
        message(FATAL_ERROR "Cannot add deploy qt: target does not exist (target: ${target})")
    endif()

    add_custom_command(
            TARGET ${target}
            POST_BUILD
            COMMAND_EXPAND_LISTS
            VERBATIM
            COMMAND
                "$<$<PLATFORM_ID:Windows>:${Qt6_DIR}/../../../bin/windeployqt.exe>"
                "$<$<PLATFORM_ID:Android>:${Qt6_DIR}/../../../bin/androiddeployqt.exe>"
                "$<$<BOOL:${arg_QML_DIR}>:--qmldir;${arg_QML_DIR}>"
                "$<IF:$<BOOL:${arg_TRANSLATIONS}>,--translations ${arg_TRANSLATIONS},--no-translations>"
                "$<TARGET_FILE:${target}>"
        )
endfunction()