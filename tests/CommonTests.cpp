// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "Common.hpp"

namespace utils {
namespace core_lib {
namespace tests {

// ComputeHash

TEST(ComputeHashTest, ShouldReturnHashWhenMessageIsNoEmpty) {
  const std::expected<std::string, std::string> sHash{
      ComputeHash(std::string{"dummyText"})};
  ASSERT_TRUE(sHash);
  EXPECT_TRUE(sHash.has_value());
}

TEST(ComputeHashTest, ShouldReturnErrorWhenMessageIsEmpty) {
  const std::expected<std::string, std::string> sHash{
      ComputeHash(std::string{})};
  ASSERT_FALSE(sHash);
  EXPECT_FALSE(sHash.has_value());
  EXPECT_EQ(sHash.error(),
            std::string{"Hash calculation failed for message: "});
}

TEST(ComputeHashTest, ShouldReturnSameHashWhenMessageIsTheSame) {
  ASSERT_EQ(ComputeHash(std::string{"dummyText"}),
            ComputeHash(std::string{"dummyText"}));
}

TEST(ComputeHashTest, ShouldReturnDifferentHashWhenMessageIsNotTheSame) {
  ASSERT_NE(ComputeHash(std::string{"dummyTextOne"}),
            ComputeHash(std::string{"dummyTextTwo"}));
}

// IsHashValid

TEST(IsHashValidTest, ShouldReturnTrueWhenHashIsValid) {
  const std::string sMessage{"dummyText"};
  const std::expected<bool, std::string> sIsHashValid{
      IsHashValid(sMessage, ComputeHash(sMessage).value())};
  ASSERT_TRUE(sIsHashValid);
  ASSERT_TRUE(sIsHashValid.value());
}

TEST(IsHashValidTest, ShouldReturnFalseWhenHashIsInvalid) {
  const std::expected<bool, std::string> sIsHashValid{
      IsHashValid(std::string{"dummyText"}, std::string{"dummyHash"})};
  ASSERT_TRUE(sIsHashValid);
  ASSERT_FALSE(sIsHashValid.value());
}

TEST(IsHashValidTest, ShouldReturnErrorWhenHashComputationFails) {
  const std::expected<bool, std::string> sIsHashValid{
      IsHashValid(std::string{}, std::string{"dummyHash"})};
  ASSERT_FALSE(sIsHashValid);
  EXPECT_EQ(sIsHashValid.error(),
            std::string{"Hash calculation failed for message: "});
}

// GetUnixTimestamp

TEST(GetUnixTimestampTest, ShouldReturnCurrentTimeWhenCallWithNoArg) {
  const std::time_t sExpectedUnixTimestamp{GetUnixTimestamp()};
  const std::time_t sActualUnixTimestamp{
      std::chrono::duration_cast<std::chrono::seconds>(
          std::chrono::system_clock::now().time_since_epoch())
          .count()};
  ASSERT_GE(sExpectedUnixTimestamp - sActualUnixTimestamp, 0);
  ASSERT_LE(sExpectedUnixTimestamp - sActualUnixTimestamp, 2);
}

TEST(GetUnixTimestampTest, ShouldReturnCorrespondingTimeWhenCallWithArg) {
  const std::time_t sPastUnixTimestamp{GetUnixTimestamp(
      std::chrono::system_clock::now() - std::chrono::minutes(300))};
  const std::time_t sCurrentUnixTimestamp{GetUnixTimestamp()};
  ASSERT_GE(sCurrentUnixTimestamp - sPastUnixTimestamp, 18000);
  ASSERT_LE(sCurrentUnixTimestamp - sPastUnixTimestamp, 18002);
}

} // namespace tests
} // namespace core_lib
} // namespace utils