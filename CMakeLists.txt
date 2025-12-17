cmake_minimum_required(VERSION 3.23)
project(CPMRecursiveTest VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download CPM. cmake
include(cmake/CPM.cmake)

# Add ProjectC (which will recursively pull ProjectB and ProjectA)
CPMAddPackage(
    NAME ProjectC
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/ProjectC
)

# Main executable
add_executable(main main.cpp)
target_link_libraries(main PRIVATE ProjectC)

message(STATUS "=== CPM Recursive Dependency Test ===")
message(STATUS "Main project successfully configured with recursive dependencies")
