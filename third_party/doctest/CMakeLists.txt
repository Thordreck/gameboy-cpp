
add_subdirectory(upstream EXCLUDE_FROM_ALL)
add_third_party_module(doctest https://github.com/doctest/doctest ${CMAKE_CURRENT_SOURCE_DIR}/upstream/LICENSE.txt)

target_compile_definitions(doctest_with_main PRIVATE DOCTEST_CONFIG_SUPER_FAST_ASSERTS DOCTEST_CONFIG_INCLUDE_TYPE_TRAITS)
include(${DOCTEST_CMAKE_HELPER})