# CPM Setup Helper Include this file from subprojects to ensure CPM is available

if(NOT COMMAND CPMAddPackage)
  # Use vendored CPM.cmake from root project
  include(${CMAKE_SOURCE_DIR}/cmake/CPM.cmake)
endif()
