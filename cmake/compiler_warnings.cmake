
function(target_compile_warnings target)
    target_compile_options(${target} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion>
    )

    set_target_properties(${target} PROPERTIES
            STATIC_LIBRARY_OPTIONS
                $<$<AND:$<BOOL:${WARNINGS_AS_ERRORS}>,$<CXX_COMPILER_ID:MSVC>,$<STREQUAL:$<TARGET_PROPERTY:${target},TYPE>,STATIC_LIBRARY>>:/Wx>
            COMPILE_WARNING_AS_ERROR
                ${WARNINGS_AS_ERRORS}
            LINK_WARNING_AS_ERROR
                ${WARNINGS_AS_ERRORS}
    )

endfunction()