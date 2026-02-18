cmake_minimum_required(VERSION 3.16)
project(hit_targets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

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

# ===================== executable ====================
add_executable(hit_targets
    ${PROJECT_SOURCES}
)

target_include_directories(hit_targets PRIVATE
    ${INCLUDE_DIR}
    ${SOURCE_DIR}
)
