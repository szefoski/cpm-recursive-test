cmake_minimum_required(VERSION 3.23)
project(
  ProjectA
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download CPM if not already available
if(NOT COMMAND CPMAddPackage)
  if(NOT EXISTS ${CMAKE_SOURCE_DIR}/cmake/get_cpm.cmake)
    file(
      DOWNLOAD
      https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.42.0/get_cpm.cmake
      ${CMAKE_SOURCE_DIR}/cmake/get_cpm.cmake)
  endif()
  include(${CMAKE_SOURCE_DIR}/cmake/get_cpm.cmake)
endif()

# External dependency (fmt library)
cpmaddpackage(
  NAME
  fmt
  GITHUB_REPOSITORY
  fmtlib/fmt
  GIT_TAG
  12.1.0
  OPTIONS
  "FMT_INSTALL YES")

cpmaddpackage(
  NAME
  googletest
  GITHUB_REPOSITORY
  google/googletest
  GIT_TAG
  v1.17.0
  VERSION
  1.17.0
  OPTIONS
  "INSTALL_GTEST OFF"
  "gtest_force_shared_crt")

# ProjectA library
add_library(ProjectA src/project_a.cpp)

target_include_directories(
  ProjectA PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

target_link_libraries(ProjectA PRIVATE fmt::fmt gtest gmock)

message(STATUS "[ProjectA] Configured with fmt dependency")
message(STATUS "[ProjectA] Called from: ${CMAKE_SOURCE_DIR}")
