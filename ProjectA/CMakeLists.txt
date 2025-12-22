cmake_minimum_required(VERSION 3.23)
project(
  ProjectA
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

# ProjectA library
add_library(ProjectA src/project_a.cpp)

target_include_directories(
  ProjectA PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

target_link_libraries(ProjectA PRIVATE fmt::fmt nlohmann_json::nlohmann_json)

message(STATUS "[ProjectA] Configured with fmt and nlohmann/json dependencies")
message(STATUS "[ProjectA] Called from: ${CMAKE_SOURCE_DIR}")
