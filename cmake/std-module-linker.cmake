
function(add_std_ifc_to_target target_name)

	if(NOT EXISTS "${CMAKE_BINARY_DIR}/std.ifc")
		execute_process(
			COMMAND
				${CMAKE_CXX_COMPILER}
				/std:c++latest
				/EHsc
				/nologo
				/W4
				/MTd
				/c
				/ifcOnly
				"$ENV{VCToolsInstallDir}/modules/std.ixx"
				/ifcOutput "${CMAKE_BINARY_DIR}"
			WORKING_DIRECTORY "${CMAKE_BINARY_DIR}"
		)
	endif()

	target_compile_options(${target_name} PRIVATE /reference "${CMAKE_BINARY_DIR}/std.ifc")

endfunction()
