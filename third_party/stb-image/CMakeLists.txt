add_library(stb_image)
target_compile_features(stb_image PUBLIC cxx_std_23)
target_include_directories(stb_image PRIVATE ${CMAKE_CURRENT_SOURCE_DIR})
set_target_properties(stb_image PROPERTIES CXX_SCAN_FOR_MODULES ON CXX_MODULE_STD ON)
target_compile_definitions(stb_image PRIVATE STB_IMAGE_IMPLEMENTATION STB_IMAGE_WRITE_IMPLEMENTATION)
target_sources(stb_image PUBLIC
        FILE_SET
        CXX_MODULES
        FILES
        ${CMAKE_CURRENT_SOURCE_DIR}/stb_image.cppm
)
