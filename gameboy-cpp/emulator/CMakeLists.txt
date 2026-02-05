
add_executable(gameboy-cpp)
target_sources(gameboy-cpp PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/emulator.cpp)
target_compile_features(gameboy-cpp PUBLIC cxx_std_23)
set_target_properties(gameboy-cpp PROPERTIES  CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(gameboy-cpp cpu)
add_std_ifc_to_target(gameboy-cpp)
