cmake_minimum_required(VERSION 3.19)
project(Static-alloc-test)
set(CMAKE_C_STANDARD 11)
set(CMAKE_BUILD_TYPE Debug)

add_subdirectory(static-alloc)
add_executable(${CMAKE_PROJECT_NAME} main.c)

# target_include_directories(static_alloc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${CMAKE_PROJECT_NAME} PUBLIC ${CMAKE_CURRENT_SOURCE_DIR}/static-alloc)
# set_property(TARGET )
add_library(static-alloc STATIC IMPORTED)
set_property(TARGET static-alloc PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}/build/static-alloc/libstatic_alloc.a)
target_link_libraries(${CMAKE_PROJECT_NAME} static-alloc)

# link_directories(${CMAKE_CURRENT_SOURCE_DIR}/static_alloc)
target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    -Wall
    -Wextra
    -Wpedantic
    -Wno-unused-parameter
    -fshort-enums
    -finline-functions
    -Og -g3 -ggdb
    # -O2 -Os -g3 -ggdb
)

target_link_options(${CMAKE_PROJECT_NAME} PRIVATE
    # -lc
    # -lm
    # -lstdc++
    # -lsupc++
)
