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

include(FetchContent)
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    message(NOTICE "Declare 1")
    FetchContent_Declare(
        raylib
        GIT_REPOSITORY https://github.com/raysan5/raylib.git
        GIT_TAG 5.0
    )
    FetchContent_GetProperties(raylib)
    if (NOT raylib_POPULATED) # Have we downloaded raylib yet?
        set(FETCHCONTENT_QUIET NO)
        FetchContent_Populate(raylib)
        set(BUILD_EXAMPLES OFF CACHE BOOL "" FORCE)
        set(BUILD_GAMES    OFF CACHE BOOL "" FORCE)
        add_subdirectory(${raylib_SOURCE_DIR} ${raylib_BINARY_DIR})
    endif()
    FetchContent_MakeAvailable(raylib)
endif()

add_subdirectory(libs/prayengine)
add_subdirectory(libs/common)

# Generate compile_commands.json
set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

include_directories(
    src/
    src/systems/
    src/utils/
    src/components/
)

file(GLOB SOURCE_FILES 
    src/*.c
    src/systems/*.c
    src/utils/*.c
    src/components/*.c
)

add_executable(${PROJECT_NAME} src/main.c ${SOURCE_FILES})

target_link_libraries(${PROJECT_NAME} PRIVATE raylib common prayengine)

