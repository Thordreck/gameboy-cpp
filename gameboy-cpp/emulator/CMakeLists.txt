
# Core
add_library(emulator-core)
add_library(emulator::core ALIAS emulator-core)
target_compile_features(emulator-core PUBLIC cxx_std_23)

set_target_properties(emulator-core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator-core PUBLIC memory joypad graphics cartridge profiling)
target_sources(emulator-core PUBLIC
		FILE_SET
		CXX_MODULES
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_core.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_common.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_tasks.cppm
)

# Gameboy
add_library(emulator-gameboy)
add_library(emulator::gameboy ALIAS emulator-gameboy)
target_compile_features(emulator-gameboy PUBLIC cxx_std_23)

set_target_properties(emulator-gameboy PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator-gameboy PUBLIC emulator::core cpu timer memory opcodes interrupts graphics joypad utilities profiling mbc cartridge)
target_sources(emulator-gameboy PUBLIC
	FILE_SET 
		CXX_MODULES 
	FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_gameboy.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_engine.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_memory.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_cpu.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_scheduler.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_factory.cppm
)

# UI
set(UI_BACKEND "SDL" CACHE STRING "UI backend")
set_property(CACHE UI_BACKEND PROPERTY STRINGS SDL Qt)

add_library(emulator-ui-backend STATIC)
target_compile_features(emulator-ui-backend PUBLIC cxx_std_23)
set_target_properties(emulator-ui-backend PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
add_library(emulator::ui::backend ALIAS emulator-ui-backend)

if(UI_BACKEND STREQUAL "SDL")
	target_compile_definitions(emulator-ui-backend PUBLIC UI_SDL)
	target_link_libraries(emulator-ui-backend PUBLIC emulator::core imgui sdl pfd profiling utilities)
	target_sources(emulator-ui-backend PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_sdl.cppm
	)
elseif(UI_BACKEND STREQUAL "Qt")
	add_library(emulator-ui-backend-qml STATIC)
	target_compile_features(emulator-ui-backend-qml PUBLIC cxx_std_23)
	target_sources(emulator-ui-backend-qml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.cpp)
	target_link_libraries(emulator-ui-backend-qml PRIVATE qt)
	set_target_properties(emulator-ui-backend-qml PROPERTIES AUTOMOC ON AUTORCC ON AUTOUIC ON)
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.qml PROPERTIES  QT_RESOURCE_ALIAS emulator_ui_qt)

	qt_add_qml_module(emulator-ui-backend-qml
			URI Emulator.UI
			VERSION ${PROJECT_VERSION}
			QML_FILES ${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.qml
			OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gameboy-cpp/Emulator/UI
			PLUGIN_TARGET emulator-ui-backend-qml-plugin
	)

	target_link_libraries(emulator-ui-backend-qml PUBLIC emulator-ui-backend-qml-plugin)

	target_compile_definitions(emulator-ui-backend PUBLIC UI_QT)
	target_link_libraries(emulator-ui-backend PRIVATE emulator::core profiling utilities qt emulator-ui-backend-qml)

	target_sources(emulator-ui-backend PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.cppm
	)

endif()

add_library(emulator-ui)
add_library(emulator::ui ALIAS emulator-ui)
target_compile_features(emulator-ui PUBLIC cxx_std_23)
set_target_properties(emulator-ui PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator-ui PUBLIC emulator::core emulator::ui::backend)
target_sources(emulator-ui PUBLIC
		FILE_SET
		CXX_MODULES
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui.cppm
)

#Joypad
set(JOYPAD_BACKEND "SDL" CACHE STRING "Joypad backend")
set_property(CACHE JOYPAD_BACKEND PROPERTY STRINGS SDL Qt)

add_library(emulator-joypad-backend STATIC)
target_compile_features(emulator-joypad-backend PUBLIC cxx_std_23)
set_target_properties(emulator-joypad-backend PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
add_library(emulator::joypad::backend ALIAS emulator-joypad-backend)

if(JOYPAD_BACKEND STREQUAL "SDL")
	target_compile_definitions(emulator-joypad-backend PUBLIC JOYPAD_SDL)
	target_link_libraries(emulator-joypad-backend PUBLIC emulator::core sdl)
	target_sources(emulator-joypad-backend PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad_sdl.cppm
	)
elseif(JOYPAD_BACKEND STREQUAL "Qt" AND UI_BACKEND STREQUAL "Qt")
	add_library(emulator-joypad-backend-qt STATIC)
	target_compile_features(emulator-joypad-backend-qt PUBLIC cxx_std_23)
	target_sources(emulator-joypad-backend-qt PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad_qt.cpp)
	target_link_libraries(emulator-joypad-backend-qt PRIVATE qt)
	set_target_properties(emulator-joypad-backend-qt PROPERTIES AUTOMOC ON)

	target_link_libraries(emulator-joypad-backend PUBLIC emulator::core profiling utilities qt emulator-joypad-backend-qt)
	target_compile_definitions(emulator-joypad-backend PUBLIC JOYPAD_QT)

	target_sources(emulator-joypad-backend PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad_qt.cppm
	)
endif()

add_library(emulator-joypad)
add_library(emulator::joypad ALIAS emulator-joypad)
target_compile_features(emulator-joypad PUBLIC cxx_std_23)
set_target_properties(emulator-joypad PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator-joypad PUBLIC emulator::core emulator::joypad::backend)
target_sources(emulator-joypad PUBLIC
		FILE_SET
		CXX_MODULES
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad.cppm
)

# Aggregator
add_library(emulator)
target_compile_features(emulator PUBLIC cxx_std_23)
set_target_properties(emulator PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator PUBLIC emulator::core emulator::gameboy emulator::ui emulator::joypad)
target_sources(emulator PUBLIC
		FILE_SET
		CXX_MODULES
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator.cppm
)

add_executable(gameboy-cpp)
target_compile_features(gameboy-cpp PUBLIC cxx_std_23)
target_link_libraries(gameboy-cpp PUBLIC emulator profiling)

target_sources(gameboy-cpp
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_main.cpp
)

# Deployment
add_custom_command(TARGET gameboy-cpp POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy -t
		$<TARGET_FILE_DIR:gameboy-cpp>
		$<TARGET_RUNTIME_DLLS:gameboy-cpp>
		COMMAND_EXPAND_LISTS
)

if (WIN32 AND (UI_BACKEND STREQUAL "Qt" OR JOYPAD_BACKEND STREQUAL "Qt"))
	add_custom_command(TARGET gameboy-cpp POST_BUILD
			COMMAND ${Qt6_DIR}/../../../bin/windeployqt.exe
			--qmldir ${Qt6_DIR}/../../../qml
			$<TARGET_FILE:gameboy-cpp>
	)
endif()