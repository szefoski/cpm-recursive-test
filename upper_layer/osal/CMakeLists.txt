cmake_minimum_required(VERSION 3.23)
project(
  osal
  VERSION 1.0.0
  LANGUAGES CXX)

cpmaddpackage(NAME fmt)

# osal library
add_library(osal src/osal.cpp)

target_include_directories(
  osal PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
              $<INSTALL_INTERFACE:include>)

target_link_libraries(osal PUBLIC crypto fmt::fmt)

message(STATUS "[osal] Configured with crypto dependency (recursive to spi)")
message(STATUS "[osal] Called from: ${CMAKE_SOURCE_DIR}")

# Unit tests
if(BUILD_TESTING)
  add_subdirectory(unit_tests)
endif()
