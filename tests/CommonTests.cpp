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

} // namespace tests
} // namespace core_lib
} // namespace utils