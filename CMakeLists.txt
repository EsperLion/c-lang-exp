cmake_minimum_required(VERSION 3.16)
project(LearnCLang C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

# -------- Main application --------
add_executable(app
    src/main.c
    src/vector.c
    src/list.c
    src/stack.c
)

# -------- Tests --------
add_executable(stack.test
    tests/stack.test.c
    src/stack.c
    src/vector.c
)
