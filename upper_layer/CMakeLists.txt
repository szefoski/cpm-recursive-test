cmake_minimum_required(VERSION 3.23)
project(
  upper_layer
  VERSION 1.0.0
  LANGUAGES CXX
)

# CPM setup for upper_layer project
include(${CMAKE_SOURCE_DIR}/cmake/CPM-dz-patched.cmake)

# Enable testing for upper_layer
if(BUILD_TESTING)
  enable_testing()
endif()

# Declare upper_layer's external dependencies
CPMDeclarePackage(
  fmt
  NAME fmt
  VERSION 12.1.0
  GITHUB_REPOSITORY fmtlib/fmt
  GIT_TAG 12.1.0 CUSTOM_CACHE_KEY "12.1.0"
  OPTIONS "FMT_INSTALL YES"
  SYSTEM YES
  GIT_SHALLOW YES
  EXCLUDE_FROM_ALL YES
)

CPMDeclarePackage(
  GTest
  NAME GTest
  VERSION 1.15.2
  GITHUB_REPOSITORY google/googletest
  GIT_TAG v1.15.2 CUSTOM_CACHE_KEY "1.15.2"
  SYSTEM YES
  GIT_SHALLOW YES
  EXCLUDE_FROM_ALL YES
  OPTIONS "INSTALL_GTEST OFF"
)

# Depend on HAL project (currently local, later will be CPM/find_package) For now, hal is already
# added by root CMakeLists.txt

# Upper layer components
add_subdirectory(osal)
