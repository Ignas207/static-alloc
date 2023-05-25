cmake_minimum_required(VERSION 3.19)
set(CMAKE_C_STANDARD 11)

add_subdirectory(static-alloc)

project(Static-alloc-test)

add_executable(${CMAKE_PROJECT_NAME} main.c)

target_link_libraries(${CMAKE_PROJECT_NAME} PUBLIC static-alloc)
# target_include_directories(static_alloc PUBLIC "${CMAKE_CURRENT_SOURCE_DIR}")
target_include_directories(${CMAKE_PROJECT_NAME} PRIVATE static-alloc)                   
