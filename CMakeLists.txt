cmake_minimum_required(VERSION 3.7)

set(CMAKE_EXPORT_COMPILE_COMMANDS on)

project(render)
add_executable(render render.c)
target_compile_options(render PRIVATE -Wall -Wextra)
