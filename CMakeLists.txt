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

# ─── Tests ────────────────────────────────────────────────────────────────────

enable_testing()

include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/tags/v1.14.0.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP TRUE
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)

add_executable(test_parser
    tests/test_parser.cpp
    src/parser.cpp
)
target_include_directories(test_parser PRIVATE include)
target_link_libraries(test_parser GTest::gtest_main)
target_compile_options(test_parser PRIVATE -Wall -Wextra)

include(GoogleTest)
gtest_discover_tests(test_parser)
