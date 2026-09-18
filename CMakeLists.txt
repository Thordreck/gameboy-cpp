cmake_minimum_required(VERSION 4.1)

#set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "d0edc3af-4c50-42ea-a356-e2862fe7a444")
set(CMAKE_EXPERIMENTAL_CXX_IMPORT_STD "f35a9ac6-8463-4d38-8eec-5d6008153e7d")
set(CMAKE_CXX_STANDARD 23)

project(gameboy-cpp VERSION 1.0 LANGUAGES CXX)

include(CMakeDependentOption)

option(BUILD_SHARED_LIBS "Build using shared libraries" OFF)
cmake_dependent_option(
        ENABLE_LTO
        "Enable Link-time optimization"
        ON
        [[CMAKE_BUILD_TYPE STREQUAL "Release" OR CMAKE_BUILD_TYPE STREQUAL "RelWithDebInfo"]]
        OFF
)
cmake_dependent_option(ENABLE_SANITIZERS "Enable sanitizers" OFF [[CMAKE_BUILD_TYPE STREQUAL "Debug"]] OFF)

if(ENABLE_LTO)
    include(CheckIPOSupported)
    check_ipo_supported(RESULT ipo_supported OUTPUT ipo_error)

    if(ipo_supported)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELEASE TRUE)
        set(CMAKE_INTERPROCEDURAL_OPTIMIZATION_RELWITHDEBINFO TRUE)

        message(VERBOSE "Enabling LTO support")
    else()
        message(WARNING "LTO not supported: ${ipo_error}")
    endif()
endif()

if(ENABLE_SANITIZERS)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/fsanitize=address>")
    add_link_options("$<$<CXX_COMPILER_ID:MSVC>:/INCREMENTAL:NO>")
endif()

if(ENABLE_LTO)
    add_compile_options("$<$<CXX_COMPILER_ID:MSVC>:/utf-8>")
endif()

include(CTest)

if(BUILD_TESTING)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/doctest)
endif()

if(UI_BACKEND STREQUAL "SDL" OR JOYPAD_BACKEND STREQUAL "SDL" OR AUDIO_BACKEND STREQUAL "SDL" OR BUILD_TESTING)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/sdl)
endif()

if(UI_BACKEND STREQUAL "SDL")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/sdl-image)
endif()

if(UI_BACKEND STREQUAL "SDL")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/imgui)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/pfd)
endif()

if(UI_BACKEND STREQUAL "Qt" OR JOYPAD_BACKEND STREQUAL "Qt" OR AUDIO_BACKEND STREQUAL "Qt")
    include(${CMAKE_CURRENT_LIST_DIR}/third_party/qt/cmake/qt_setup.cmake NO_POLICY_SCOPE)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/qt)
endif()

if(PROFILING_BACKEND STREQUAL "Tracy")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/tracy)
elseif(PROFILING_BACKEND STREQUAL "CTrack")
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/ctrack)
endif()

add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/boost)

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
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/serial)
add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gameboy-cpp/emulator)

if(BUILD_TESTING)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/stb-image)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/plot)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/svg)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/third_party/dsp)

    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/blargg-test-roms)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/mooneye-test-suite)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/dmg-acid2-test-rom)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/scribbl-test-roms)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/gbmicrotest-suite)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/mealybug-tearoom-tests)
    add_subdirectory(${CMAKE_CURRENT_LIST_DIR}/not-chciken-test-roms)
endif()