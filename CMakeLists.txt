cmake_minimum_required(VERSION 3.25)
project(untitled1 C)

set(CMAKE_C_STANDARD 11)

add_executable(untitled1 main.c)
target_link_libraries(${CMAKE_PROJECT_NAME} ws2_32)
