# Crypto Unit Tests
cmake_minimum_required(VERSION 3.23)

if(NOT TARGET gtest_main)
  cpmaddpackage(NAME GTest)
endif()

if(TARGET gtest_main)
  add_executable(crypto_test crypto_test.cpp)
  target_link_libraries(crypto_test PRIVATE crypto gtest_main)

  include(GoogleTest)
  gtest_discover_tests(crypto_test)

  message(STATUS "[crypto] Unit tests configured")
else()
  message(WARNING "[crypto] GTest not available, skipping unit tests")
endif()
