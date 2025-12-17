cmake_minimum_required(VERSION 3.23)
project(
  CPMRecursiveTest
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download CPM if not already available
if(NOT COMMAND CPMAddPackage)
  file(
    DOWNLOAD
    https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.7/CPM.cmake
    ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
    cmake
    EXPECTED_HASH
      SHA256=83e5eb71b2bbb8b1f2ad38f1950287a057624e385c238f6087f94cdfc44af9c5)
  include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake)
endif()

# Add ProjectC (which will recursively pull ProjectB and ProjectA)
cpmaddpackage(NAME ProjectC SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ProjectC)

# Main executable
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ProjectC)

message(STATUS "=== CPM Recursive Dependency Test ===")
message(
  STATUS "Main project successfully configured with recursive dependencies")
