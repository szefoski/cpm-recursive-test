#include "crypto.hpp"
#include <gtest/gtest.h>

using namespace hal::crypto;

class CryptoTest : public ::testing::Test {
protected:
  Crypto crypto;
};

TEST_F(CryptoTest, GetInfoReturnsCorrectString) {
  auto info = crypto.get_info();
  EXPECT_FALSE(info.empty());
  EXPECT_TRUE(info.find("crypto") != std::string::npos);
}

TEST_F(CryptoTest, ProcessWithSpiReturnsProcessedString) {
  auto result = crypto.process_with_spi("data");
  EXPECT_FALSE(result.empty());
  EXPECT_TRUE(result.find("crypto") != std::string::npos);
  EXPECT_TRUE(result.find("data") != std::string::npos);
}

TEST_F(CryptoTest, ProcessWithSpiUsesUnderlyingSpi) {
  auto result = crypto.process_with_spi("test");
  // Should contain evidence of spi processing
  EXPECT_TRUE(result.find("spi") != std::string::npos);
}
