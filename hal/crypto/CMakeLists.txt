# crypto library
add_library(crypto src/crypto.cpp)

target_compile_features(crypto PUBLIC cxx_std_23)
set_target_properties(crypto PROPERTIES CXX_EXTENSIONS OFF)

target_include_directories(
  crypto PUBLIC $<BUILD_INTERFACE: ${CMAKE_CURRENT_SOURCE_DIR}/include>
                $<INSTALL_INTERFACE:include>)

target_link_libraries(crypto PUBLIC spi)

message(STATUS "[crypto] Configured with spi dependency")
message(STATUS "[crypto] Called from: ${CMAKE_SOURCE_DIR}")

# Unit tests
if(BUILD_TESTING)
  add_subdirectory(unit_tests)
endif()
