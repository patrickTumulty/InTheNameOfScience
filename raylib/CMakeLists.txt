cmake_minimum_required(VERSION 3.28)

project(raylib LANGUAGES C)

set(RAYLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/raylib-5.5_linux_amd64)

add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)

set_target_properties(${PROJECT_NAME} PROPERTIES
    IMPORTED_LOCATION "${RAYLIB_DIR}/lib/libraylib.a"
    INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_DIR}/include"
    INTERFACE_LINK_LIBRARIES "m;pthread;dl;GL;X11" # Unix
)

