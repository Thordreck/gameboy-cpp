
# Core
add_library(emulator)
target_compile_features(emulator PUBLIC cxx_std_23)
set_target_properties(emulator PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator PUBLIC engine memory joypad graphics cartridge audio)
target_compile_warnings(emulator)

target_sources(emulator PUBLIC
	FILE_SET 
		CXX_MODULES
	FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_common.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_audio.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_tasks.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_gameboy.cppm
)

# UI
set(UI_BACKEND "SDL" CACHE STRING "UI backend")
set_property(CACHE UI_BACKEND PROPERTY STRINGS SDL Qt)

if(UI_BACKEND STREQUAL "SDL")
	target_compile_definitions(emulator PUBLIC UI_SDL)
	target_link_libraries(emulator-ui PRIVATE imgui sdl sdl-image pfd)
	target_sources(emulator PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_sdl.cppm
	)

elseif(UI_BACKEND STREQUAL "Qt")
	add_library(emulator-qml STATIC)
	target_compile_warnings(emulator-qml)
	target_compile_features(emulator-qml PUBLIC cxx_std_23)
	target_sources(emulator-qml PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.cpp)
	target_link_libraries(emulator-qml PRIVATE Qt6::Core Qt6::Qml Qt6::Quick Boost::lockfree)
	set_target_properties(emulator-qml PROPERTIES AUTOMOC ON AUTORCC ON AUTOUIC ON)
	set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.qml PROPERTIES
			QT_RESOURCE_ALIAS emulator_ui_qt
			QT_QML_SOURCE_TYPENAME EmulatorUI
	)
	set_source_files_properties(${CMAKE_SOURCE_DIR}/resources/icons/gameboy-icon.png PROPERTIES  QT_RESOURCE_ALIAS gameboy-icon.png)

	qt_add_qml_module(emulator-qml
			URI Gameboy.UI
			VERSION ${PROJECT_VERSION}
			QML_FILES emulator_ui_qt.qml
			OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/gameboy-cpp/Gameboy/UI
			PLUGIN_TARGET emulator-qml-plugin
			DEPENDENCIES QtCore QtQuick
			IMPORTS ${QT_QUICK_CONTROLS}
	)

	if(CMAKE_BUILD_TYPE MATCHES "Debug|RelWithDebInfo")
		set_source_files_properties(${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.qml PROPERTIES QT_QML_SKIP_CACHEGEN TRUE)
		target_compile_definitions(emulator-qml PUBLIC QML_HOT_RELOAD_PATH="${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.qml")
		target_compile_definitions(emulator-qml PUBLIC QT_UI_DEBUG_MODE)
	endif()

	qt_add_resources(emulator-qml "icons"
			PREFIX "/icons"
			FILES ${CMAKE_SOURCE_DIR}/resources/icons/gameboy-icon.png
	)

	qt_add_resources(emulator-qml "qtquickcontrols2.conf" FILES qtquickcontrols2.conf)
	set_target_properties(emulator-qml PROPERTIES QT_QMLCACHEGEN_ARGUMENTS "--verbose")
	set_target_properties(emulator-qml PROPERTIES QT_QMLCACHEGEN_DIRECT_CALLS ON)
	target_link_libraries(emulator-qml PRIVATE emulator-qml-plugin)
	target_compile_definitions(emulator-qml PRIVATE "$<$<CONFIG:Debug>:QT_QML_DEBUG>")
	target_compile_definitions(emulator-qml PRIVATE "$<$<CONFIG:RelWithDebInfo>:QT_QML_DEBUG>")

	target_sources(emulator PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_qt.cppm
	)

	target_link_libraries(emulator PRIVATE emulator-qml)
	target_compile_definitions(emulator PUBLIC UI_QT)
endif()

#Joypad
set(JOYPAD_BACKEND "SDL" CACHE STRING "Joypad backend")
set_property(CACHE JOYPAD_BACKEND PROPERTY STRINGS SDL Qt)

if(JOYPAD_BACKEND STREQUAL "SDL")
	target_compile_definitions(emulator PUBLIC JOYPAD_SDL)
	target_link_libraries(emulator PRIVATE sdl)
	target_sources(emulator PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad_sdl.cppm
	)

elseif(JOYPAD_BACKEND STREQUAL "Qt" AND UI_BACKEND STREQUAL "Qt")
	target_compile_definitions(emulator PUBLIC JOYPAD_QT)
	target_link_libraries(emulator PRIVATE qt)
	set_target_properties(emulator PROPERTIES AUTOMOC ON)

	target_sources(emulator PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad_qt.cppm
	)
endif()

#Audio
set(AUDIO_BACKEND "SDL" CACHE STRING "Audio backend")
set_property(CACHE AUDIO_BACKEND PROPERTY STRINGS SDL Qt)

if(AUDIO_BACKEND STREQUAL "SDL")
	target_compile_definitions(emulator PUBLIC AUDIO_SDL)
	target_link_libraries(emulator PRIVATE sdl)
	target_sources(emulator PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_audio_sdl.cppm
	)
elseif(AUDIO_BACKEND STREQUAL "Qt")
	target_compile_definitions(emulator PUBLIC AUDIO_QT)
	target_link_libraries(emulator PRIVATE Qt6::Multimedia audio utilities)
	target_sources(emulator PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_audio_qt.cppm
	)
endif()