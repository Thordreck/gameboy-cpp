
set(BOOST_INCLUDE_LIBRARIES "lockfree;uuid" CACHE STRING "" FORCE)
set(BOOST_ENABLE_MPI OFF CACHE BOOL "" FORCE)
set(BOOST_ENABLE_PYTHON OFF CACHE BOOL "" FORCE)

add_third_party_subdirectory(upstream)
add_third_party_module(Boost https://github.com/boostorg/boost ${CMAKE_CURRENT_SOURCE_DIR}/upstream/LICENSE_1_0.txt)
