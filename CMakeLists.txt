cmake_minimum_required(VERSION 3.13)
project(interpreter C)

set(CMAKE_C_STANDARD 99)

add_library(interpreter SHARED src/c/interpreter.c src/c/interpreter.h)