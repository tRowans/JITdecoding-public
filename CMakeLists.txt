cmake_minimum_required(VERSION 3.10)

project(JITdecoding)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
#set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(src)
add_subdirectory(lib/blossom5)
add_subdirectory(lib/googletest)
add_subdirectory(tst)
