cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE ON)
set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RelWithDebInfo ON)

project(gameboy-cpp VERSION 1.0 LANGUAGES CXX)
include(CTest)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    include(${CMAKE_CURRENT_LIST_DIR}/third_party/doctest/cmake/doctest-target.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/third_party/doctest/cmake/doctest.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/sdl)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/cpu)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/memory)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/utilities)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/opcodes)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/interrupts)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/timer)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/graphics)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/joypad)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/audio)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/cartridge)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/mbc)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/profiling)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/emulator)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/stb-image)

    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/blargg-test-roms)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/mooneye-test-suite)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/dmg-acid2-test-rom)
endif()