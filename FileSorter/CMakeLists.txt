cmake_minimum_required(VERSION 3.5)

project(FileSorter)
set(CMAKE_CXX_FLAGS "-Wall -Wextra -fopenmp -O3 ${CMAKE_CXX_FLAGS}")

if ("${CMAKE_CXX_COMPILER_ID}" STREQUAL "GNU")
    set(CMAKE_CXX_FLAGS "-std=gnu++17 ${CMAKE_CXX_FLAGS}")
else ()
    set(CMAKE_CXX_FLAGS "-std=c++17 ${CMAKE_CXX_FLAGS}")
endif()

add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} stdc++fs)

