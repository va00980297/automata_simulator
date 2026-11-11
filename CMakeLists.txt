cmake_minimum_required(VERSION 3.16)
project(AutomataSimulator LANGUAGES CXX)

# Use C++20, no compiler-specific extensions
set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# Warnings for all targets below
add_compile_options(-Wall -Wextra)

# The core code, compiled once as a library
add_library(automata
    src/Automaton.cpp
    src/DFA.cpp
    src/NFA.cpp
)
target_include_directories(automata PUBLIC include)

# Demo program
add_executable(demo src/main.cpp)
target_link_libraries(demo PRIVATE automata)

# Test program
add_executable(run_tests tests/tests.cpp)
target_link_libraries(run_tests PRIVATE automata)

# Register the test so `ctest` can run it
enable_testing()
add_test(NAME automata_tests COMMAND run_tests)