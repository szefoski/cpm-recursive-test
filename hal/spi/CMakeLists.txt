# External dependencies - versions declared at root level
cpmaddpackage(NAME fmt)
cpmaddpackage(NAME nlohmann_json)

# spi library
add_library(spi src/spi.cpp)

target_compile_features(spi PUBLIC cxx_std_23)
set_target_properties(spi PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(
  spi PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
             $<INSTALL_INTERFACE:include>)

target_link_libraries(spi PRIVATE fmt::fmt nlohmann_json::nlohmann_json)

message(STATUS "[spi] Configured with fmt and nlohmann/json dependencies")
message(STATUS "[spi] Called from: ${CMAKE_SOURCE_DIR}")

# Unit tests
if(BUILD_TESTING)
  add_subdirectory(unit_tests)
endif()
