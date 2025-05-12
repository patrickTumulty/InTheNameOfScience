cmake_minimum_required(VERSION 3.10)

project(prayengine VERSION 0.1.0 LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True) # Optional, but recommended

include(FetchContent)
find_package(raylib QUIET)
if (NOT raylib_FOUND)
    message(NOTICE "Declare")
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
endif()

file(GLOB SOURCES
    src/pray_component.c
    src/pray_camera.c
    src/pray_engine.c
    src/pray_default_components.c
    src/pray_entity_registry.c
    src/pray_default_systems.c
    src/pray_entity.c
    src/pray_system.c
    src/pray_utils.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PUBLIC raylib common)

find_package(PkgConfig REQUIRED)
pkg_check_modules(CUNIT REQUIRED cunit)

add_executable(${PROJECT_NAME}_test
    tests/main.c
    tests/test_components.c
    tests/entity_tests.c
)

target_include_directories(${PROJECT_NAME}_test PUBLIC
    ${CUNIT_INCLUDE_DIRS}
    tests
)

target_link_libraries(${PROJECT_NAME}_test PUBLIC prayengine ${CUNIT_LIBRARIES})

