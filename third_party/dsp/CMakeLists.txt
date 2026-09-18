
set(DSPLIB_NO_EXCEPTIONS ON CACHE BOOL "" FORCE)
set(DSPLIB_FFT_BACKEND ON CACHE STRING "fftw" FORCE)
set(DSPLIB_ENABLE_LTO OFF CACHE BOOL "" FORCE)
set(DSPLIB_USE_FLOAT32 ON CACHE BOOL "" FORCE)

add_subdirectory(upstream EXCLUDE_FROM_ALL)

add_library(dsp)
target_compile_features(dsp PUBLIC cxx_std_23)
set_target_properties(dsp PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_link_libraries(dsp PRIVATE dsplib)

target_sources(dsp PUBLIC
        FILE_SET
        CXX_MODULES
        BASE_DIRS
        ${CMAKE_CURRENT_SOURCE_DIR}
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/dsp.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/common.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/fft.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/math.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/window.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/correlation.cppm
        ${CMAKE_CURRENT_SOURCE_DIR}/utils.cppm
)
