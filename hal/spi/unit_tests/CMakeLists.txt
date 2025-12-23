# SPI Unit Tests
cmake_minimum_required(VERSION 3.23)

if(NOT TARGET gtest_main)
  cpmaddpackage(NAME GTest)
endif()

if(TARGET gtest_main)
  add_executable(spi_test spi_test.cpp)
  target_link_libraries(spi_test PRIVATE spi gtest_main)

  include(GoogleTest)
  gtest_discover_tests(spi_test)

  message(STATUS "[spi] Unit tests configured")
else()
  message(WARNING "[spi] GTest not available, skipping unit tests")
endif()
