#include "spi.hpp"
#include <gtest/gtest.h>

using namespace hal::spi;

class SpiTest : public ::testing::Test {
protected:
  Spi spi;
};

TEST_F(SpiTest, GetInfoReturnsCorrectString) {
  auto info = spi.get_info();
  EXPECT_FALSE(info.empty());
  EXPECT_TRUE(info.find("spi") != std::string::npos);
}

TEST_F(SpiTest, FormatMessageReturnsFormattedString) {
  auto result = spi.format_message("test");
  EXPECT_FALSE(result.empty());
  EXPECT_TRUE(result.find("spi") != std::string::npos);
  EXPECT_TRUE(result.find("test") != std::string::npos);
}

TEST_F(SpiTest, FormatMessageWithEmptyString) {
  auto result = spi.format_message("");
  EXPECT_FALSE(result.empty());
}
