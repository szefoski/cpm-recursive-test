cmake_minimum_required(VERSION 3.23)
project(
  ProjectC
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

if(NOT COMMAND CPMAddPackage)
  include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)
endif()

# External dependencies - versions declared at root level
cpmaddpackage(NAME fmt)
cpmaddpackage(NAME nlohmann_json)

# Dependency on ProjectB (which depends on spi - recursive!) Path declared at
# root level, we just request by NAME
cpmaddpackage(NAME ProjectB)

# ProjectC library
add_library(ProjectC src/project_c.cpp)

target_include_directories(
  ProjectC PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

target_link_libraries(ProjectC PUBLIC ProjectB fmt::fmt
                                      nlohmann_json::nlohmann_json)

message(
  STATUS "[ProjectC] Configured with ProjectB dependency (recursive to spi)")
message(STATUS "[ProjectC] Called from: ${CMAKE_SOURCE_DIR}")
