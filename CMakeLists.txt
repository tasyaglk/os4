cmake_minimum_required(VERSION 3.24)
project(os4 C)

set(CMAKE_C_STANDARD 11)

add_executable(os4 main.c cmake-build-debug/4-5/teacher.c cmake-build-debug/4-5/students.c)
