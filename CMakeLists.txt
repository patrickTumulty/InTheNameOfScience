cmake_minimum_required(VERSION 3.28)

project(itnos LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True) # Optional, but recommended

add_subdirectory(raylib)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)

set(
    SOURCE_FILES
    src/astar.c
    src/linked_list.c
    src/bool_mat.c
    src/utils.c
)

add_executable(${PROJECT_NAME} src/main.c ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib)

add_executable(playground src/playground.c src/linked_list.c src/bool_mat.c src/astar.c src/utils.c)

target_link_libraries(playground PRIVATE m)

