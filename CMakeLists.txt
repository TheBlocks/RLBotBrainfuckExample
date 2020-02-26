cmake_minimum_required(VERSION 3.13)
project(interpreter C)

set(CMAKE_C_STANDARD 99)

set(DEBUG_OPTIONS "-g")
set(RELEASE_OPTIONS "-Ofast")

add_library(interpreter SHARED src/c/interpreter.c src/c/interpreter.h)

target_compile_options(interpreter PUBLIC "$<$<CONFIG:RELEASE>:${DEBUG_OPTIONS}>")
target_compile_options(interpreter PUBLIC "$<$<CONFIG:RELEASE>:${RELEASE_OPTIONS}>")