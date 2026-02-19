cmake_minimum_required(VERSION 3.16)
project(hit_targets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)


set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

# ===================== includes ====================
set(INCLUDE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/include")
set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

include_directories(
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

file(GLOB_RECURSE PROJECT_SOURCES
    "${INCLUDE_DIR}/*.h"
    "${SOURCE_DIR}/*.cpp"
)

# ===================== quad_tree_lib ====================
add_library(quad_tree_lib
    ${SOURCE_DIR}/quad_tree.cpp
)

target_include_directories(quad_tree_lib PRIVATE
    ${INCLUDE_DIR}
)

# ===================== executable ====================
add_executable(hit_targets
    ${SOURCE_DIR}/main.cpp
)

target_include_directories(hit_targets PRIVATE
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)

target_link_libraries(hit_targets PRIVATE
    quad_tree_lib
)

# ===================== tests ====================
option(BUILD_TESTS "Build hit_targets tests" ON)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
