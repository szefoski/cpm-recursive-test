cpmaddpackage(NAME fmt)

# osal library
add_library(osal src/osal.cpp)

target_compile_features(osal PUBLIC cxx_std_23)
set_target_properties(osal PROPERTIES CXX_EXTENSIONS OFF)

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
