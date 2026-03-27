add_executable(dmg_acid2_test)
target_compile_features(dmg_acid2_test PUBLIC cxx_std_23)
target_link_libraries(dmg_acid2_test PRIVATE emulator stb_image doctest::doctest_with_main)
set_target_properties(dmg_acid2_test PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
doctest_discover_tests(dmg_acid2_test)

target_sources(dmg_acid2_test
        PRIVATE
        ${CMAKE_CURRENT_SOURCE_DIR}/dmg_acid2.test.cpp
)

# Copy ROMs to build directory
file(GLOB_RECURSE TEST_ROMS "${CMAKE_CURRENT_SOURCE_DIR}/rom/*.gb")

foreach(ROM ${TEST_ROMS})
    configure_file(${ROM}
            ${CMAKE_CURRENT_BINARY_DIR}
            COPYONLY)
endforeach()

# Copy any references images to build directory
file(GLOB_RECURSE REFERENCE_IMAGES "${CMAKE_CURRENT_SOURCE_DIR}/*.png")

foreach(IMG ${REFERENCE_IMAGES})
    configure_file(${IMG}
            ${CMAKE_CURRENT_BINARY_DIR}
            COPYONLY)
endforeach()
