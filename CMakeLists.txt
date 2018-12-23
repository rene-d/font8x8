cmake_minimum_required(VERSION 3.7)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)
add_compile_options( -Wall -Wextra)

add_executable(render render.c)

add_executable(arduino arduino.cpp)
