add_library(audio)
target_compile_features(audio PUBLIC cxx_std_23)

set_target_properties(audio PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(audio memory)
target_sources(audio PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/audio.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_apu.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_memory.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_channels.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/audio_processing.cppm
)
