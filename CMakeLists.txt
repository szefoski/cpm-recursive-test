cmake_minimum_required(VERSION 3.23)
project(
  CPMRecursiveTest
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CPM_USE_NAMED_CACHE_DIRECTORIES ON)

# Set CPM source cache to speed up builds and share dependencies
set(CPM_SOURCE_CACHE
    "${CMAKE_SOURCE_DIR}/.cpm-cache"
    CACHE PATH "CPM source cache directory")

# Include vendored CPM.cmake (v0.42.0) for reproducibility and offline builds To
# upgrade: download new CPM.cmake from
# https://github.com/cpm-cmake/CPM.cmake/releases
if(NOT COMMAND CPMAddPackage)
  include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)
endif()

# Declare all local subprojects upfront (directory-structure agnostic)
# Subprojects can request these by NAME without knowing their location
cpmdeclarepackage(ProjectA NAME ProjectA SOURCE_DIR
                  ${CMAKE_CURRENT_SOURCE_DIR}/ProjectA)

cpmdeclarepackage(ProjectB NAME ProjectB SOURCE_DIR
                  ${CMAKE_CURRENT_SOURCE_DIR}/ProjectB)

cpmdeclarepackage(ProjectC NAME ProjectC SOURCE_DIR
                  ${CMAKE_CURRENT_SOURCE_DIR}/ProjectC)

# Add ProjectC (which will recursively pull ProjectB and ProjectA)
cpmaddpackage(NAME ProjectC)

# Main executable
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ProjectC)

message(STATUS "=== CPM Recursive Dependency Test ===")
message(
  STATUS "Main project successfully configured with recursive dependencies")
