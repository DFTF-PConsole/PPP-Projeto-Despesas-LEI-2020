cmake_minimum_required(VERSION 3.15.3)
project(Trabalho_Final C)

set(CMAKE_C_STANDARD 99)

set(GCC_COVERAGE_COMPILE_FLAGS "-Wall -Wextra")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} ${GCC_COVERAGE_COMPILE_FLAGS}")

add_executable(Trabalho_Final main.c trabalho.h estruturaDeDados.c)