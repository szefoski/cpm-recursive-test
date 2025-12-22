cmake_minimum_required(VERSION 3.23)
project(
  CPMRecursiveTest
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

set(CPM_USE_NAMED_CACHE_DIRECTORIES ON)

# Set CPM source cache to speed up builds and share dependencies Note:
# SOURCE_CACHE only applies to downloaded packages, not local SOURCE_DIR
# packages
set(CPM_SOURCE_CACHE
    "${CMAKE_SOURCE_DIR}/.cpm-cache"
    CACHE PATH "CPM source cache directory")

include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# Declare all external dependencies upfront for version consistency
cpmdeclarepackage(
  fmt
  NAME
  fmt
  VERSION
  12.1.0
  GITHUB_REPOSITORY
  fmtlib/fmt
  GIT_TAG
  12.1.0
  CUSTOM_CACHE_KEY
  "12.1.0"
  OPTIONS
  "FMT_INSTALL YES")

cpmdeclarepackage(
  nlohmann_json
  NAME
  nlohmann_json
  VERSION
  3.11.3
  GITHUB_REPOSITORY
  nlohmann/json
  GIT_TAG
  v3.11.3
  CUSTOM_CACHE_KEY
  "3.11.3")

# Include HAL components (after external dependencies)
add_subdirectory(hal)

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
