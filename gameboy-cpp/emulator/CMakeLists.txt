
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
	add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/imgui ${CMAKE_BINARY_DIR}/third_party/imgui)
	add_subdirectory(${CMAKE_SOURCE_DIR}/third_party/pfd ${CMAKE_BINARY_DIR}/third_party/pfd)

	target_compile_definitions(emulator-ui-backend PUBLIC UI_SDL)
	target_link_libraries(emulator-ui-backend PUBLIC emulator::core imgui sdl pfd profiling utilities)
	target_sources(emulator-ui-backend PUBLIC
			FILE_SET
			CXX_MODULES
			FILES
			${CMAKE_CURRENT_SOURCE_DIR}/emulator_ui_sdl.cppm
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

# Aggregator
add_library(emulator)
target_compile_features(emulator PUBLIC cxx_std_23)
set_target_properties(emulator PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator PUBLIC emulator::core emulator::gameboy emulator::ui)
target_sources(emulator PUBLIC
		FILE_SET
		CXX_MODULES
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_joypad.cppm
)

add_executable(gameboy-cpp)
target_compile_features(gameboy-cpp PUBLIC cxx_std_23)
target_link_libraries(gameboy-cpp PRIVATE emulator profiling)

target_sources(gameboy-cpp 
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_main.cpp
)

add_custom_command(TARGET gameboy-cpp POST_BUILD
		COMMAND ${CMAKE_COMMAND} -E copy_if_different
		$<TARGET_RUNTIME_DLLS:gameboy-cpp>
		$<TARGET_FILE_DIR:gameboy-cpp>
		COMMAND_EXPAND_LISTS
)
