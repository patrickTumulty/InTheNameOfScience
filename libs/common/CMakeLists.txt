cmake_minimum_required(VERSION 3.28)

project(common LANGUAGES C)

set(CMAKE_C_STANDARD 23)
set(CMAKE_C_STANDARD_REQUIRED True) # Optional, but recommended

set(
    SOURCE_FILES
    src/linked_list.c
    src/bool_mat.c
    src/common_utils.c
    src/pointer_list.c
    src/tmem.c
    src/array_list.c
    src/common_random.c
)

add_library(${PROJECT_NAME} STATIC ${SOURCE_FILES})
target_include_directories(${PROJECT_NAME} PUBLIC include)
target_link_libraries(${PROJECT_NAME} PRIVATE m)

# Enable testing
enable_testing()

# Find CUnit
find_package(PkgConfig REQUIRED)
pkg_check_modules(CUNIT REQUIRED cunit)

add_executable(${PROJECT_NAME}_test
    tests/main.c
    tests/linked_list_tests.c
    tests/array_list_tests.c
    tests/boolmat_tests.c
    tests/tmem_tests.c
    tests/utils_tests.c
)

target_include_directories(${PROJECT_NAME}_test PUBLIC 
    ${CUNIT_INCLUDE_DIRS}
    tests 
)

target_link_libraries(${PROJECT_NAME}_test PUBLIC common ${CUNIT_LIBRARIES})

