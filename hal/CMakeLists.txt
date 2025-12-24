cmake_minimum_required(VERSION 3.23)
project(
  hal
  VERSION 1.0.0
  LANGUAGES CXX)

# CPM setup for HAL project
include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)

# Enable testing for HAL
if(BUILD_TESTING)
  enable_testing()
endif()

# Declare HAL's external dependencies
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
  "FMT_INSTALL YES"
  SYSTEM
  YES
  GIT_SHALLOW
  YES
  GIT_PROGRESS
  YES
  EXCLUDE_FROM_ALL
  YES)

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
  "3.11.3-patched"
  PATCHES
  "${CMAKE_CURRENT_LIST_DIR}/patches/nlohmann_json/01-add-custom-comment.patch"
  "${CMAKE_CURRENT_LIST_DIR}/patches/nlohmann_json/02-customize-namespace.patch"
  SYSTEM
  YES
  GIT_SHALLOW
  YES
  GIT_PROGRESS
  YES
  EXCLUDE_FROM_ALL
  YES)

cpmdeclarepackage(
  GTest
  NAME
  GTest
  VERSION
  1.15.2
  GITHUB_REPOSITORY
  google/googletest
  GIT_TAG
  v1.15.2
  CUSTOM_CACHE_KEY
  "1.15.2"
  SYSTEM
  YES
  GIT_SHALLOW
  YES
  GIT_PROGRESS
  YES
  EXCLUDE_FROM_ALL
  YES
  OPTIONS
  "INSTALL_GTEST OFF")

# HAL components - local subdirectories
add_subdirectory(spi)
add_subdirectory(crypto)
