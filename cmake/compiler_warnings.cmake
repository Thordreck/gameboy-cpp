
function(target_compile_warnings target)
    target_compile_options(${target} PRIVATE
            $<$<CXX_COMPILER_ID:MSVC>:/W4>
            $<$<CXX_COMPILER_ID:Clang>:-Wall -pedantic-errors -Wall -Wextra -Wconversion -Wsign-conversion>
    )
endfunction()