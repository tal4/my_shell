cmake_minimum_required(VERSION 3.16)
project(my_shell CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

add_executable(my_shell
    src/main.cpp
    src/parser.cpp
    src/executor.cpp
)

target_include_directories(my_shell PRIVATE include)

target_compile_options(my_shell PRIVATE -Wall -Wextra -Wpedantic)