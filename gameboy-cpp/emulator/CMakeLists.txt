
# Core
add_library(emulator-core)
add_library(emulator::core ALIAS emulator-core)
target_compile_features(emulator-core PUBLIC cxx_std_23)

set_target_properties(emulator-core PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator-core PUBLIC cpu timer memory opcodes interrupts graphics utilities)
target_sources(emulator-core PUBLIC
	FILE_SET 
		CXX_MODULES 
	FILES 
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_core.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_memory.cppm
		${CMAKE_CURRENT_SOURCE_DIR}/emulator_cpu.cppm
)

# UI
add_library(emulator-ui)
add_library(emulator::ui ALIAS emulator-ui)
target_compile_features(emulator-ui PUBLIC cxx_std_23)
set_target_properties(emulator-ui PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(emulator-ui PUBLIC imgui sdl pfd utilities)
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
target_link_libraries(emulator PUBLIC emulator::core emulator::ui)
target_sources(emulator PUBLIC
		FILE_SET
		CXX_MODULES
		FILES
		${CMAKE_CURRENT_SOURCE_DIR}/emulator.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/emulator_adapters.cppm
)

add_executable(gameboy-cpp)
target_compile_features(gameboy-cpp PUBLIC cxx_std_23)
target_link_libraries(gameboy-cpp PRIVATE emulator)

target_sources(gameboy-cpp 
	PRIVATE 
		${CMAKE_CURRENT_SOURCE_DIR}/main.cpp
)