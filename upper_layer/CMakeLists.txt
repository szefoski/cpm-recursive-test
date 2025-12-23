cmake_minimum_required(VERSION 3.23)
project(
  upper_layer
  VERSION 1.0.0
  LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Upper layer components
add_subdirectory(osal)
