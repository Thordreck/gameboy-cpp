cmake_minimum_required(VERSION 4.1)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(gameboy-cpp VERSION 1.0 LANGUAGES CXX)
include(CTest)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    include(${CMAKE_CURRENT_LIST_DIR}/third_party/doctest/cmake/doctest-target.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/third_party/doctest/cmake/doctest.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/imgui)

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/cpu)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/memory)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/utilities)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/opcodes)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/interrupts)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/timer)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/graphics)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/emulator)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/blargg-test-roms)
endif()