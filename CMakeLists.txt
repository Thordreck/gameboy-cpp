cmake_minimum_required(VERSION 4.2)

set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")

project(gameboy-cpp VERSION 1.0 LANGUAGES CXX)
include(CTest)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/std-module-linker.cmake)
include(${CMAKE_CURRENT_LIST_DIR}/cmake/generate-ifc-dt-files.cmake)

if(PROJECT_IS_TOP_LEVEL AND BUILD_TESTING)
    include(${CMAKE_CURRENT_LIST_DIR}/third_party/doctest/cmake/doctest-target.cmake)
    include(${CMAKE_CURRENT_LIST_DIR}/third_party/doctest/cmake/doctest.cmake)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/utilities)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/cpu)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/emulator)

generate_ifc_dt_files()
