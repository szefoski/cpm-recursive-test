cmake_minimum_required(VERSION 3.23)
project(
  ProjectB
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT COMMAND CPMAddPackage)
  include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)
endif()

# Dependency on ProjectA (recursive dependency) Path declared at root level, we
# just request by NAME
cpmaddpackage(NAME ProjectA)

# ProjectB library
add_library(ProjectB src/project_b.cpp)

target_include_directories(
  ProjectB PUBLIC $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

target_link_libraries(ProjectB PUBLIC ProjectA)

message(STATUS "[ProjectB] Configured with ProjectA dependency")
message(STATUS "[ProjectB] Called from: ${CMAKE_SOURCE_DIR}")
