add_library(doctest INTERFACE)
add_library(doctest::doctest ALIAS doctest)
target_include_directories(doctest INTERFACE ${CMAKE_CURRENT_LIST_DIR}/../include)

add_library(doctest_with_main)
add_library(doctest::doctest_with_main ALIAS doctest_with_main)
target_link_libraries(doctest_with_main PUBLIC doctest::doctest)
target_compile_definitions(doctest_with_main PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS)
target_sources(doctest_with_main PRIVATE ${CMAKE_CURRENT_LIST_DIR}/../src/doctest_main.cpp)