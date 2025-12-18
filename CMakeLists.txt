cmake_minimum_required(VERSION 3.23)
project(
  CPMRecursiveTest
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Set CPM source cache to speed up builds and share dependencies
set(CPM_SOURCE_CACHE
    "${CMAKE_SOURCE_DIR}/.cpm-cache"
    CACHE PATH "CPM source cache directory")

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

# Add ProjectC (which will recursively pull ProjectB and ProjectA)
cpmaddpackage(NAME ProjectC SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ProjectC)

# Main executable
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ProjectC)

message(STATUS "=== CPM Recursive Dependency Test ===")
message(
  STATUS "Main project successfully configured with recursive dependencies")
