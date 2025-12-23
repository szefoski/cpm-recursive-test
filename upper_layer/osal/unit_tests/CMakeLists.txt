# OSAL Unit Tests
cmake_minimum_required(VERSION 3.23)

if(NOT TARGET gtest_main)
  cpmaddpackage(NAME GTest)
endif()

if(TARGET gtest_main)
  add_executable(osal_test osal_test.cpp)
  target_link_libraries(osal_test PRIVATE osal gtest_main)

  include(GoogleTest)
  gtest_discover_tests(osal_test)

  message(STATUS "[osal] Unit tests configured")
else()
  message(WARNING "[osal] GTest not available, skipping unit tests")
endif()
