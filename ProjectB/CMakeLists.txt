cmake_minimum_required(VERSION 3.23)
project(
  ProjectB
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

# Dependency on ProjectA (recursive dependency)
cpmaddpackage(NAME ProjectA SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ProjectA)

# ProjectB library
add_library(ProjectB src/project_b.cpp)

target_include_directories(
  ProjectB PUBLIC $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

target_link_libraries(ProjectB PUBLIC ProjectA)

message(STATUS "[ProjectB] Configured with ProjectA dependency")
message(STATUS "[ProjectB] Called from: ${CMAKE_SOURCE_DIR}")
