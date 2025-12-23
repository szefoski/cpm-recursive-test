cmake_minimum_required(VERSION 3.23)
project(
  spi
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# External dependencies - versions declared at root level
cpmaddpackage(NAME fmt)
cpmaddpackage(NAME nlohmann_json)

# Validate dependencies with advanced rules
cpm_valid_version(spi fmt "12.1.0" "12.2.1" "12.3.0...12.3.6" "!12.3.4")
cpm_valid_version(spi nlohmann_json "3.11.3")

# spi library
add_library(spi src/spi.cpp)

target_include_directories(
  spi PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
             $<INSTALL_INTERFACE:include>)

target_link_libraries(spi PRIVATE fmt::fmt nlohmann_json::nlohmann_json)

message(STATUS "[spi] Configured with fmt and nlohmann/json dependencies")
message(STATUS "[spi] Called from: ${CMAKE_SOURCE_DIR}")
