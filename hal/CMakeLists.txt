cmake_minimum_required(VERSION 3.23)
project(
  hal
  VERSION 1.0.0
  LANGUAGES CXX)

# HAL components - local subdirectories
add_subdirectory(spi)
add_subdirectory(crypto)
