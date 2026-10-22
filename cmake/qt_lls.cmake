
function(generate_qmlls_configuration_file)
    configure_file(
            ${CMAKE_SOURCE_DIR}/cmake/qmlls.ini.in
            ${CMAKE_SOURCE_DIR}/.qmlls.ini)
endfunction()