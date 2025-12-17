cmake_minimum_required(VERSION 3.23)
project(ProjectC VERSION 1.0.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Download CPM if not already available
if(NOT COMMAND CPMAddPackage)
    file(DOWNLOAD
         https://github.com/cpm-cmake/CPM.cmake/releases/download/v0.38.7/CPM.cmake
         ${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM.cmake
         EXPECTED_HASH SHA256=83e5eb71b2bbb8b1f2ad38f1950287a057624e385c238f6087f94cdfc44af9c5
    )
    include(${CMAKE_CURRENT_BINARY_DIR}/cmake/CPM. cmake)
endif()

# Dependency on ProjectB (which depends on ProjectA - recursive!)
CPMAddPackage(
    NAME ProjectB
    SOURCE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/../ProjectB
)

# ProjectC library
add_library(ProjectC
    src/project_c.cpp
)

target_include_directories(ProjectC
    PUBLIC
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
)

target_link_libraries(ProjectC PUBLIC ProjectB)

message(STATUS "[ProjectC] Configured with ProjectB dependency (recursive to ProjectA)")
