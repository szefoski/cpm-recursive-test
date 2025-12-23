cmake_minimum_required(VERSION 3.23)
project(
  hal
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# HAL components - local subdirectories
add_subdirectory(spi)
add_subdirectory(crypto)
