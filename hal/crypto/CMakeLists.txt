cmake_minimum_required(VERSION 3.23)
project(
  crypto
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# crypto library
add_library(crypto src/crypto.cpp)

target_include_directories(
  crypto PUBLIC $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>)

target_link_libraries(crypto PUBLIC spi)

message(STATUS "[crypto] Configured with spi dependency")
message(STATUS "[crypto] Called from: ${CMAKE_SOURCE_DIR}")
