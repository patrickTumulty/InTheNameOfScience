cmake_minimum_required(VERSION 3.28)

project(itnos LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True) # Optional, but recommended

if(NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug CACHE STRING "Build type" FORCE)
endif()

if(CMAKE_BUILD_TYPE STREQUAL "Debug")
    add_compile_definitions(TMEM_TRACK)
endif()

add_subdirectory(libs/raylib)
add_subdirectory(libs/common)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(src)

set(
    SOURCE_FILES
    src/astar.c
    src/sim.c
    src/object_registry.c
    src/system.c
    src/components.c
    src/entity.c
)

add_executable(${PROJECT_NAME} src/main.c ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib common)

