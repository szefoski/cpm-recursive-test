cmake_minimum_required(VERSION 3.23)
project(
  ProjectB
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# ProjectB library
add_library(ProjectB src/project_b.cpp)

target_include_directories(
  ProjectB PUBLIC $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
                  $<INSTALL_INTERFACE:include>)

target_link_libraries(ProjectB PUBLIC spi)

message(STATUS "[ProjectB] Configured with spi dependency")
message(STATUS "[ProjectB] Called from: ${CMAKE_SOURCE_DIR}")
