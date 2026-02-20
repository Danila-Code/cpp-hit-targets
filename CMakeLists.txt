cmake_minimum_required(VERSION 3.16)
project(hit_targets)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_CXX_EXTENSIONS OFF)

# ===================== includes ====================
#set(SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/src")

#include_directories(
#    ${SOURCE_DIR}
#)

#file(GLOB_RECURSE PROJECT_SOURCES
#    "${SOURCE_DIR}/*.cpp"
#)

# ===================== quad_tree_lib ====================
add_subdirectory(quad_tree_lib)

# ===================== hit_targets_gui ====================
add_subdirectory(hit_targets_gui)

# ===================== executable ====================
#add_executable(hit_targets
#    ${SOURCE_DIR}/main.cpp
#)

#target_include_directories(hit_targets PRIVATE
#    ${SOURCE_DIR}
#)

#target_link_libraries(hit_targets PRIVATE
#    quad_tree_lib
#)

# ===================== tests ====================
option(BUILD_TESTS "Build hit_targets tests" OFF)
if(BUILD_TESTS)
    add_subdirectory(tests)
endif()
