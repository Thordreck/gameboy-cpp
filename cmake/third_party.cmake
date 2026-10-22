function(add_third_party_module name url license)
    if(NOT IS_READABLE ${license})
        message(FATAL_ERROR "Third party module {name}'s license file is not readable (path: ${license})")
    endif()

    get_property(THIRD_PARTY_MODULES_METADATA GLOBAL PROPERTY third_party_modules_metadata_prop)

    if(NOT DEFINED THIRD_PARTY_MODULES_METADATA)
        set(THIRD_PARTY_MODULES_METADATA "{ \"modules\": []}")
    endif()

    cmake_parse_arguments(PARSE_ARGV 3 arg "" "NOTES" "")

    set(entry "{}")
    string(JSON entry SET ${entry} name \"${name}\")
    string(JSON entry SET ${entry} url \"${url}\")
    string(JSON entry SET ${entry} license \"${license}\")

    if(DEFINED arg_NOTES)
        string(JSON entry SET ${entry} notes \"${arg_NOTES}\")
    endif()

    string(JSON num_modules LENGTH "${THIRD_PARTY_MODULES_METADATA}" "modules")
    string(JSON THIRD_PARTY_MODULES_METADATA SET "${THIRD_PARTY_MODULES_METADATA}" "modules" "${num_modules}" "${entry}")

    set_property(GLOBAL PROPERTY third_party_modules_metadata_prop "${THIRD_PARTY_MODULES_METADATA}")
endfunction()

function(register_third_party_module_license_processor PROCESSOR)
    set_property(GLOBAL PROPERTY third_party_modules_license_processor_prop ${PROCESSOR})
endfunction()

function(generate_third_party_module)
    get_property(THIRD_PARTY_MODULES_METADATA GLOBAL PROPERTY third_party_modules_metadata_prop)
    get_property(THIRD_PARTY_MODULES_LICENSE_PROCESSOR GLOBAL PROPERTY third_party_modules_license_processor_prop)

    if(NOT DEFINED THIRD_PARTY_MODULES_METADATA)
        set(THIRD_PARTY_MODULES_METADATA "{ \"modules\": []}")
    endif()

    if(DEFINED THIRD_PARTY_MODULES_LICENSE_PROCESSOR)
        string(JSON num_modules LENGTH "${THIRD_PARTY_MODULES_METADATA}" modules)
        math(EXPR num_modules "${num_modules} - 1")

        foreach(module_index RANGE ${num_modules})
            string(JSON license_file GET "${THIRD_PARTY_MODULES_METADATA}" modules ${module_index} license)
            cmake_language(CALL ${THIRD_PARTY_MODULES_LICENSE_PROCESSOR} ${license_file} converted_license_file)
            string(JSON THIRD_PARTY_MODULES_METADATA SET "${THIRD_PARTY_MODULES_METADATA}" modules ${module_index} license \"${converted_license_file}\")
        endforeach()
    endif()

    configure_file(
            ${CMAKE_SOURCE_DIR}/cmake/third_party_metadata.cppm.in
            ${CMAKE_BINARY_DIR}/generated/third_party_metadata.cppm)
endfunction()

function(link_third_party_module target)
    target_sources(${target} PUBLIC
            FILE_SET
            CXX_MODULES
            BASE_DIRS
            ${CMAKE_BINARY_DIR}
            FILES
            ${CMAKE_BINARY_DIR}/generated/third_party_metadata.cppm
    )
endfunction()

function(add_third_party_subdirectory directory)
    cmake_diagnostic(PUSH)
    cmake_diagnostic(SET CMD_UNINITIALIZED IGNORE)

    add_subdirectory(${directory} EXCLUDE_FROM_ALL SYSTEM)

    cmake_diagnostic(POP)
endfunction()
