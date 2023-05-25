cmake_minimum_required(VERSION 3.19)

set(CMAKE_C_STANDARD 11)
# set(CMAKE_SYSTEM_NAME Generic)
# set(CMAKE_TRY_COMPILE_TARGET_TYPE "STATIC_LIBRARY")

project(static_alloc VERSION 1.0 DESCRIPTION "A simple static memory allocation")
add_library(static_alloc SHARED static_alloc.c)

set_target_properties(static_alloc PROPERTIES
                    VERSION ${PROJECT_VERSION}
                    PUBLIC_HEADER static_alloc.h)


# add_subdirectory(static-alloc)
# target_include_directories(static_alloc PUBLIC .)
# target_sources(static_alloc PRIVATE "${CMAKE_CURRENT_SOURCE_DIR}/static_alloc.c")
