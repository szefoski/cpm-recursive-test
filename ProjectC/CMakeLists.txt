cmake_minimum_required(VERSION 3.23)
project(
  ProjectC
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

cpmaddpackage(
  NAME
  fmt
  GITHUB_REPOSITORY
  fmtlib/fmt
  GIT_TAG
  10.2.1
  CUSTOM_CACHE_KEY
  "10.2.1"
  OPTIONS
  "FMT_INSTALL YES")

# Dependency on ProjectB (which depends on ProjectA - recursive!)
cpmaddpackage(NAME ProjectB SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ProjectB)

# ProjectC library
add_library(ProjectC src/project_c.cpp)

target_include_directories(
  ProjectC PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

target_link_libraries(ProjectC PUBLIC ProjectB fmt::fmt)

message(
  STATUS
    "[ProjectC] Configured with ProjectB dependency (recursive to ProjectA)")
message(STATUS "[ProjectC] Called from: ${CMAKE_SOURCE_DIR}")
