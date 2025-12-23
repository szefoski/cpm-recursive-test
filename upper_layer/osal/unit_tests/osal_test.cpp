#include "osal.hpp"
#include <gtest/gtest.h>

using namespace upper_layer::osal;

class OsalTest : public ::testing::Test {
protected:
  Osal osal;
};

TEST_F(OsalTest, GetInfoReturnsCorrectString) {
  auto info = osal.get_info();
  EXPECT_FALSE(info.empty());
  EXPECT_TRUE(info.find("osal") != std::string::npos);
}

TEST_F(OsalTest, ExecuteReturnsProcessedCommand) {
  auto result = osal.execute("command");
  EXPECT_FALSE(result.empty());
  EXPECT_TRUE(result.find("osal") != std::string::npos);
  EXPECT_TRUE(result.find("command") != std::string::npos);
}

TEST_F(OsalTest, ExecuteUsesUnderlyingCrypto) {
  auto result = osal.execute("test");
  // Should contain evidence of crypto processing
  EXPECT_TRUE(result.find("crypto") != std::string::npos);
}

TEST_F(OsalTest, ExecuteWithEmptyCommand) {
  auto result = osal.execute("");
  EXPECT_FALSE(result.empty());
}
