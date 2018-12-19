cmake_minimum_required(VERSION 3.5)

project(FileSorter)
set(CMAKE_CXX_FLAGS "-std=c++17 -Wall -Wextra -lstdc++fs ${CMAKE_CXX_FLAGS}")
add_executable(${PROJECT_NAME} "main.cpp")
target_link_libraries(${PROJECT_NAME} stdc++fs)
