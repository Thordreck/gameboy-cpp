function(get_git_commit_hash output)
    cmake_parse_arguments(PARSE_ARGV 1 arg "ERROR_IS_FATAL" "RESULT_VARIABLE" "")
    find_package(Git)

    if(Git_FOUND)
        execute_process(
                COMMAND ${GIT_EXECUTABLE} rev-parse --short HEAD
                OUTPUT_VARIABLE commit
                RESULT_VARIABLE result
                ERROR_QUIET
                OUTPUT_STRIP_TRAILING_WHITESPACE
        )

        if(DEFINED arg_RESULT_VARIABLE)
            set(${arg_RESULT_VARIABLE} ${result} PARENT_SCOPE)
        endif()

        if(DEFINED arg_ERROR_IS_FATAL AND NOT result EQUAL 0)
            message(FATAL_ERROR "Git command failed with error code ${result}")
        endif()

        set(${output} ${commit} PARENT_SCOPE)
    elseif(DEFINED ERROR_IS_FATAL)
        message(FATAL_ERROR "Git executable not found")
    endif()
endfunction()