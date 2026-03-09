cmake_minimum_required(VERSION 3.10)
project(MyShell VERSION 1.0 LANGUAGES CXX)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -Wall -Wextra")
include_directories(include)
file(GLOB SOURCES "src/*.cpp")
add_executable(my_shell ${SOURCES})
include(FetchContent)
FetchContent_Declare(
    googletest
    URL https://github.com/google/googletest/archive/refs/heads/main.zip
)
set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable(googletest)
add_executable(run_tests
    tests/test_parser.cpp
    src/parser.cpp
)
target_link_libraries(run_tests GTest::gtest_main)
target_include_directories(run_tests PRIVATE include)


