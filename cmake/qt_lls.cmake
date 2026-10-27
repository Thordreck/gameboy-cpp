
function(generate_qmlls_configuration_file)
    if(NOT IS_READABLE ${CMAKE_SOURCE_DIR}/cmake/qmlls.ini.in)
        message(FATAL_ERROR "qmlls template file is not readable (path: ${CMAKE_SOURCE_DIR}/cmake/qmlls.ini.in)")
    endif()

    configure_file(
            ${CMAKE_SOURCE_DIR}/cmake/qmlls.ini.in
            ${CMAKE_SOURCE_DIR}/.qmlls.ini)
endfunction()