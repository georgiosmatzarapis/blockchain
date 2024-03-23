// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "User.hpp"

namespace user {
namespace tests {

class UserProfileTest : public ::testing::Test {
 protected:
  UserProfileTest()
      : _testData{{"fullName", std::string{"Username"}},
                  {"age", static_cast<std::uint8_t>(30)},
                  {"deposit", 1.2}},
        _profile{std::get<std::string>(_testData["fullName"]),
                 std::get<std::uint8_t>(_testData["age"]),
                 std::get<double>(_testData["deposit"])} {};

  std::map<std::string, std::variant<std::string, std::uint8_t, double>>
      _testData{};
  user::Profile _profile;
};

class UserInMemoryDatabaseTest : public UserProfileTest {
 protected:
  UserInMemoryDatabaseTest()
      : _profileId{user::InMemoryDatabase::GetInstance().insert(_profile)},
        _profileIdValue{_profileId.second.value()} {};

  const std::pair<bool, std::optional<std::uint16_t>> _profileId{};
  const std::uint16_t _profileIdValue{};
};

/* === Profile Tests === */

TEST_F(UserProfileTest, ShouldReturnFullName) {
  ASSERT_EQ(_profile.getFullName(),
            std::get<std::string>(_testData["fullName"]));
}

TEST_F(UserProfileTest, ShouldReturnAge) {
  ASSERT_EQ(_profile.getAge(), std::get<std::uint8_t>(_testData["age"]));
}

TEST_F(UserProfileTest, ShouldReturnDeposit) {
  ASSERT_EQ(_profile.getDeposit(), std::get<double>(_testData["deposit"]));
}

TEST_F(UserProfileTest, ShouldUpdateDepositWhenNewDepositIsPositive) {
  constexpr double sAmount{1.3};
  bool sIsDepositUpdated{_profile.updateDeposit(sAmount)};
  ASSERT_TRUE(sIsDepositUpdated);
  EXPECT_EQ(_profile.getDeposit(),
            (sAmount + std::get<double>(_testData["deposit"])));
}

TEST_F(UserProfileTest, ShouldUpdateDepositWhenNewDepositIsZero) {
  constexpr double sAmount{-1.2};
  bool sIsDepositUpdated{_profile.updateDeposit(sAmount)};
  ASSERT_TRUE(sIsDepositUpdated);
  EXPECT_EQ(_profile.getDeposit(), 0);
}

TEST_F(UserProfileTest, ShouldNotUpdateDepositWhenNewDepositIsNegative) {
  constexpr double sAmount{-1.3};
  bool sIsDepositUpdated{_profile.updateDeposit(sAmount)};
  ASSERT_FALSE(sIsDepositUpdated);
  EXPECT_EQ(_profile.getDeposit(), std::get<double>(_testData["deposit"]));
}

/* === InMemoryDatabase Tests === */

TEST_F(UserInMemoryDatabaseTest, ShouldInsertProfileWhenRecordDoesNotExist) {
  ASSERT_TRUE(_profileId.first);
  ASSERT_EQ(_profileId.second, 0);
}

TEST_F(UserInMemoryDatabaseTest,
       ShouldIncreaseUniqueIdWhenNewProfileIsInserted) {
  std::pair<bool, std::optional<std::uint16_t>> sProfileId{
      user::InMemoryDatabase::GetInstance().insert(_profile)};
  EXPECT_TRUE(_profileId.first);
  EXPECT_EQ(_profileId.second, 0);
  ASSERT_TRUE(sProfileId.first);
  ASSERT_EQ(sProfileId.second, 1);
}

TEST_F(UserInMemoryDatabaseTest, ShouldGetProfileWhenPassedIdIsValid) {
  std::pair<bool, std::optional<user::Profile>> sRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  const user::Profile sRetrievedProfileValue{sRetrievedProfile.second.value()};
  ASSERT_TRUE(sRetrievedProfile.first);
  EXPECT_EQ(sRetrievedProfileValue.getFullName(), _profile.getFullName());
  EXPECT_EQ(sRetrievedProfileValue.getAge(), _profile.getAge());
  EXPECT_EQ(sRetrievedProfileValue.getDeposit(), _profile.getDeposit());
}

TEST_F(UserInMemoryDatabaseTest, ShouldNotGetProfileWhenPassedIdIsInvalid) {
  std::pair<bool, std::optional<user::Profile>> sRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(1)};
  ASSERT_FALSE(sRetrievedProfile.first);
}

TEST_F(UserInMemoryDatabaseTest, ShouldRemoveProfileWhenPassedIdIsValid) {
  bool sIsProfileRemoved{
      user::InMemoryDatabase::GetInstance().remove(_profileIdValue)};
  ASSERT_TRUE(sIsProfileRemoved);
  ASSERT_FALSE(
      user::InMemoryDatabase::GetInstance().get(_profileIdValue).first);
}

TEST_F(UserInMemoryDatabaseTest, ShouldNotRemoveProfileWhenPassedIdIsInvalid) {
  bool sIsProfileRemoved{user::InMemoryDatabase::GetInstance().remove(1)};
  ASSERT_FALSE(sIsProfileRemoved);
}

TEST_F(UserInMemoryDatabaseTest, ShouldUpdateProfileWhenPassedIdIsValid) {
  constexpr double sAmount{1.2};
  _profile.updateDeposit(sAmount);
  bool sIsProfileUpdated{
      user::InMemoryDatabase::GetInstance().update(_profileIdValue, _profile)};
  ASSERT_TRUE(sIsProfileUpdated);
  std::pair<bool, std::optional<user::Profile>> sRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  EXPECT_EQ(sRetrievedProfile.second.value().getDeposit(),
            (std::get<double>(_testData["deposit"]) + sAmount));

  // Update with a completely new instance
  user::InMemoryDatabase::GetInstance().update(
      _profileIdValue, user::Profile{"NewUser", 25, 1.5});
  std::pair<bool, std::optional<user::Profile>> sNewlyRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  const user::Profile sNewlyRetrievedProfileValue{
      sNewlyRetrievedProfile.second.value()};
  ASSERT_EQ(sNewlyRetrievedProfileValue.getFullName(), "NewUser");
  ASSERT_EQ(sNewlyRetrievedProfileValue.getAge(), 25);
  ASSERT_EQ(sNewlyRetrievedProfileValue.getDeposit(), 1.5);
}

TEST_F(UserInMemoryDatabaseTest, ShouldNotUpdateProfileWhenPassedIdIsInvalid) {
  bool sIsProfileUpdated{user::InMemoryDatabase::GetInstance().update(
      1, user::Profile{"NewUser", 25, 1.5})};
  ASSERT_FALSE(sIsProfileUpdated);
  std::pair<bool, std::optional<user::Profile>> sRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  const user::Profile sRetrievedProfileValue{sRetrievedProfile.second.value()};
  ASSERT_EQ(sRetrievedProfileValue.getFullName(),
            std::get<std::string>(_testData["fullName"]));
  ASSERT_EQ(sRetrievedProfileValue.getAge(),
            std::get<std::uint8_t>(_testData["age"]));
  ASSERT_EQ(sRetrievedProfileValue.getDeposit(),
            std::get<double>(_testData["deposit"]));
}
} // namespace tests
} // namespace user