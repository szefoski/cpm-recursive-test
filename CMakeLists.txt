cmake_minimum_required(VERSION 4.2.1)
project(
  CPMRecursiveTest
  VERSION 1.0.0
  LANGUAGES CXX
)

set(CMAKE_COLOR_DIAGNOSTICS ON)

# Enable testing - creates BUILD_TESTING option (OFF by default)
option(BUILD_TESTING "Build the testing tree" OFF)
if(BUILD_TESTING)
  enable_testing()
endif()

# Shared CPM cache for monorepo
set(CPM_SOURCE_CACHE
    "${CMAKE_SOURCE_DIR}/.cpm-cache"
    CACHE PATH "CPM source cache directory"
)

# CPM setup for application - only the final consumer uses lock file
include(cmake/CPM-dz-patched.cmake)
CPMUsePackageLock(package-lock.cmake)

# Monorepo aggregator - each subproject manages its own dependencies
add_subdirectory(hal)
add_subdirectory(upper_layer)
add_subdirectory(src)

message(STATUS "=== CPM Recursive Dependency Test ===")
message(STATUS "Main project successfully configured with recursive dependencies")
