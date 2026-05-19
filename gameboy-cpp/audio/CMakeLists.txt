add_library(audio)
target_compile_features(audio PUBLIC cxx_std_23)

set_target_properties(audio PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(audio memory utilities)
target_sources(audio PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/audio.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_dac.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_apu.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_memory.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_channels.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_processing.cppm
)
if(BUILD_TESTING)
    add_executable(audio_tests)
    target_compile_features(audio_tests PUBLIC cxx_std_23)
    target_link_libraries(audio_tests PRIVATE audio doctest::doctest_with_main)
    set_target_properties(audio_tests PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
    doctest_discover_tests(audio_tests)

    target_sources(audio_tests
            PRIVATE
            ${CMAKE_CURRENT_SOURCE_DIR}/audio_dac.test.cpp
    )

endif()
