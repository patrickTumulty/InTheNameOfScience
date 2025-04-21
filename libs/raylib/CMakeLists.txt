cmake_minimum_required(VERSION 3.28)

###########################################################
# Download Raylib library from GitHub releases
# Project structure looks like this
#
# raylib
# ├── CMakeLists.txt
# └── raylib-5.5_linux_amd64
#     ├── CHANGELOG
#     ├── include
#     │   ├── raylib.h
#     │   ├── raymath.h
#     │   └── rlgl.h
#     ├── lib
#     │   ├── libraylib.a
#     │   ├── libraylib.so -> libraylib.so.550
#     │   ├── libraylib.so.5.5.0
#     │   └── libraylib.so.550 -> libraylib.so.5.5.0
#     ├── LICENSE
#     └── README.md
###########################################################

project(raylib LANGUAGES C)

set(RAYLIB_DIR ${CMAKE_CURRENT_SOURCE_DIR}/raylib-5.5_linux_amd64)

add_library(${PROJECT_NAME} STATIC IMPORTED GLOBAL)

set_target_properties(${PROJECT_NAME} PROPERTIES
    IMPORTED_LOCATION "${RAYLIB_DIR}/lib/libraylib.a"
    INTERFACE_INCLUDE_DIRECTORIES "${RAYLIB_DIR}/include"
    INTERFACE_LINK_LIBRARIES "m;pthread;dl;GL;X11" # Unix
)

