
add_subdirectory(upstream EXCLUDE_FROM_ALL SYSTEM)
add_third_party_module(ctrack https://github.com/Compaile/ctrack ${CMAKE_CURRENT_SOURCE_DIR}/upstream/LICENSE)
