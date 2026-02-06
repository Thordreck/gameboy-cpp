add_library(doctest INTERFACE)
add_library(doctest::doctest ALIAS doctest)

target_include_directories(doctest
    INTERFACE
        ${CMAKE_CURRENT_LIST_DIR}/../include
)

add_library(doctest_with_main INTERFACE)
add_library(doctest::doctest_with_main ALIAS doctest_with_main)
target_link_libraries(doctest_with_main INTERFACE doctest::doctest)
target_compile_definitions(doctest_with_main 
    INTERFACE 
        DOCTEST_CONFIG_IMPLEMENT_WITH_MAIN 
        DOCTEST_CONFIG_SUPER_FAST_ASSERTS)