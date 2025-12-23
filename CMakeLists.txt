cmake_minimum_required(VERSION 3.23)
project(
  CPMRecursiveTest
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_COLOR_DIAGNOSTICS ON)

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

add_subdirectory(hal)
add_subdirectory(upper_layer)
add_subdirectory(src)

message(STATUS "=== CPM Recursive Dependency Test ===")
message(
  STATUS "Main project successfully configured with recursive dependencies")
