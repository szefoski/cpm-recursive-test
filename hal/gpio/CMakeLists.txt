cmake_minimum_required(VERSION 3.23)
project(
  gpio
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# gpio library
add_library(gpio src/project_b.cpp)

target_include_directories(
  gpio PUBLIC $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>)

target_link_libraries(gpio PUBLIC spi)

message(STATUS "[gpio] Configured with spi dependency")
message(STATUS "[gpio] Called from: ${CMAKE_SOURCE_DIR}")
