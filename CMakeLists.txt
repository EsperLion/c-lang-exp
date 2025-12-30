cmake_minimum_required(VERSION 4.2.1)
project(LearnCLang C)

set(CMAKE_C_STANDARD 11)

include_directories(include)

add_executable(app
    src/main.c
    src/vector.c
    src/list.c
)

# target_sources(app
#     PRIVATE
#     src/main.c
# )
