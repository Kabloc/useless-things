cmake_minimum_required(VERSION 3.10)
project (logger)

set (CMAKE_CXX_STANDARD 17)
set (CMAKE_CXX_STANDARD_REQUIRED ON)
set (CMAKE_CXX_EXTENSIONS OFF)

find_package (Threads)
add_definitions (-DMULTITHREAD_LOGGER)

add_executable (logger main.cpp)
target_link_libraries (logger ${CMAKE_THREAD_LIBS_INIT})
