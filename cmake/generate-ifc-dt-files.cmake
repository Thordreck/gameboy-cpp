
function(generate_ifc_dt_files)

	file(GLOB IFC_COMMAND_FILES "${CMAKE_BINARY_DIR}/*.ifc.dt.command")

	foreach(cmd_file IN LISTS IFC_COMMAND_FILES)

		execute_process(
			COMMAND ${CMAKE_CXX_COMPILER} @${cmd_file}
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
			COMMAND_ECHO STDOUT
		)
	endforeach()

endfunction()