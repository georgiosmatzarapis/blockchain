// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "User.hpp"

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
  EXPECT_EQ(_profile.getFullName(),
            std::get<std::string>(_testData["fullName"]));
}

TEST_F(UserProfileTest, ShouldReturnAge) {
  EXPECT_EQ(_profile.getAge(), std::get<std::uint8_t>(_testData["age"]));
}

TEST_F(UserProfileTest, ShouldReturnDeposit) {
  EXPECT_EQ(_profile.getDeposit(), std::get<double>(_testData["deposit"]));
}

TEST_F(UserProfileTest, ShouldUpdateDepositWhenNewDepositIsPositive) {
  constexpr double aAmount{1.3};
  bool aIsDepositUpdated{_profile.updateDeposit(aAmount)};
  ASSERT_TRUE(aIsDepositUpdated);
  EXPECT_EQ(_profile.getDeposit(),
            (aAmount + std::get<double>(_testData["deposit"])));
}

TEST_F(UserProfileTest, ShouldUpdateDepositWhenNewDepositIsZero) {
  constexpr double aAmount{-1.2};
  bool aIsDepositUpdated{_profile.updateDeposit(aAmount)};
  ASSERT_TRUE(aIsDepositUpdated);
  EXPECT_EQ(_profile.getDeposit(), 0);
}

TEST_F(UserProfileTest, ShouldNotUpdateDepositWhenNewDepositIsNegative) {
  constexpr double aAmount{-1.3};
  bool aIsDepositUpdated{_profile.updateDeposit(aAmount)};
  ASSERT_FALSE(aIsDepositUpdated);
  EXPECT_EQ(_profile.getDeposit(), std::get<double>(_testData["deposit"]));
}

/* === InMemoryDatabase Tests === */

TEST_F(UserInMemoryDatabaseTest, ShouldInsertProfileWhenRecordDoesNotExist) {
  ASSERT_TRUE(_profileId.first);
  ASSERT_EQ(_profileId.second, 0);
}

TEST_F(UserInMemoryDatabaseTest,
       ShouldIncreaseUniqueIdWhenNewProfileIsInserted) {
  std::pair<bool, std::optional<std::uint16_t>> aProfileId{
      user::InMemoryDatabase::GetInstance().insert(_profile)};
  EXPECT_TRUE(_profileId.first);
  EXPECT_EQ(_profileId.second, 0);
  ASSERT_TRUE(aProfileId.first);
  ASSERT_EQ(aProfileId.second, 1);
}

TEST_F(UserInMemoryDatabaseTest, ShouldGetProfileWhenPassedIdIsValid) {
  std::pair<bool, std::optional<user::Profile>> aRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  const user::Profile aRetrievedProfileValue{aRetrievedProfile.second.value()};
  ASSERT_TRUE(aRetrievedProfile.first);
  EXPECT_EQ(aRetrievedProfileValue.getFullName(), _profile.getFullName());
  EXPECT_EQ(aRetrievedProfileValue.getAge(), _profile.getAge());
  EXPECT_EQ(aRetrievedProfileValue.getDeposit(), _profile.getDeposit());
}

TEST_F(UserInMemoryDatabaseTest, ShouldNotGetProfileWhenPassedIdIsInvalid) {
  std::pair<bool, std::optional<user::Profile>> aRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(1)};
  ASSERT_FALSE(aRetrievedProfile.first);
}

TEST_F(UserInMemoryDatabaseTest, ShouldRemoveProfileWhenPassedIdIsValid) {
  bool aIsProfileRemoved{
      user::InMemoryDatabase::GetInstance().remove(_profileIdValue)};
  ASSERT_TRUE(aIsProfileRemoved);
  ASSERT_FALSE(
      user::InMemoryDatabase::GetInstance().get(_profileIdValue).first);
}

TEST_F(UserInMemoryDatabaseTest, ShouldNotRemoveProfileWhenPassedIdIsInvalid) {
  bool aIsProfileRemoved{user::InMemoryDatabase::GetInstance().remove(1)};
  ASSERT_FALSE(aIsProfileRemoved);
}

TEST_F(UserInMemoryDatabaseTest, ShouldUpdateProfileWhenPassedIdIsValid) {
  constexpr double aAmount{1.2};
  _profile.updateDeposit(aAmount);
  bool aIsProfileUpdated{
      user::InMemoryDatabase::GetInstance().update(_profileIdValue, _profile)};
  ASSERT_TRUE(aIsProfileUpdated);
  std::pair<bool, std::optional<user::Profile>> aRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  EXPECT_EQ(aRetrievedProfile.second.value().getDeposit(),
            (std::get<double>(_testData["deposit"]) + aAmount));

  // Update with a completely new instance
  user::InMemoryDatabase::GetInstance().update(
      _profileIdValue, user::Profile{"NewUser", 25, 1.5});
  std::pair<bool, std::optional<user::Profile>> aNewlyRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  const user::Profile aNewlyRetrievedProfileValue{
      aNewlyRetrievedProfile.second.value()};
  EXPECT_EQ(aNewlyRetrievedProfileValue.getFullName(), "NewUser");
  EXPECT_EQ(aNewlyRetrievedProfileValue.getAge(), 25);
  EXPECT_EQ(aNewlyRetrievedProfileValue.getDeposit(), 1.5);
}

TEST_F(UserInMemoryDatabaseTest, ShouldNotUpdateProfileWhenPassedIdIsInvalid) {
  bool aIsProfileUpdated{user::InMemoryDatabase::GetInstance().update(
      1, user::Profile{"NewUser", 25, 1.5})};
  ASSERT_FALSE(aIsProfileUpdated);
  std::pair<bool, std::optional<user::Profile>> aRetrievedProfile{
      user::InMemoryDatabase::GetInstance().get(_profileIdValue)};
  const user::Profile aRetrievedProfileValue{aRetrievedProfile.second.value()};
  EXPECT_EQ(aRetrievedProfileValue.getFullName(),
            std::get<std::string>(_testData["fullName"]));
  EXPECT_EQ(aRetrievedProfileValue.getAge(),
            std::get<std::uint8_t>(_testData["age"]));
  EXPECT_EQ(aRetrievedProfileValue.getDeposit(),
            std::get<double>(_testData["deposit"]));
}