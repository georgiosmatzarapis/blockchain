// author: georgiosmatzarapis

#include "User.hpp"
#include "Logger.hpp"

namespace user {

using namespace utils;

static const Log& sLog{Log::GetInstance()};

/* === Profile Class === */

Profile::Profile(std::string fullName, const std::uint8_t& age,
                 const double& deposit = 0)
    : _fullName{std::move(fullName)},
      _age{age},
      _deposit{deposit} {}

// Public API

std::string Profile::getFullName() const { return _fullName; }

std::uint8_t Profile::getAge() const { return _age; }

double Profile::getDeposit() const { return _deposit; }

bool Profile::updateDeposit(const double iAmount) {
  const double aUpdatedDeposit{_deposit + iAmount};
  return [&]() -> bool {
    if (aUpdatedDeposit >= 0) {
      _deposit = aUpdatedDeposit;
      sLog.toFile(LogLevel::INFO,
                  "Deposit for user '" + _fullName + "' is updated.",
                  __PRETTY_FUNCTION__);
      return true;
    }

    sLog.toFile(LogLevel::WARNING,
                "Invalid amount to update deposit of user '" + _fullName + "'.",
                __PRETTY_FUNCTION__);
    return false;
  }();
}

/* === InMemoryDatabase Class === */

InMemoryDatabase::InMemoryDatabase() = default;

InMemoryDatabase::~InMemoryDatabase() = default;

// Public API

InMemoryDatabase& InMemoryDatabase::GetInstance() {
  static InMemoryDatabase sInstance{};
  return sInstance;
}

std::pair<bool, std::optional<std::uint16_t>>
InMemoryDatabase::insert(Profile iProfile) {
  std::uint16_t aProfileId{idGenerator()};
  const auto aRecordIterator{_profiles.emplace(
      aProfileId, std::make_unique<Profile>(std::move(iProfile)))};

  if (aRecordIterator.second) {
    sLog.toFile(LogLevel::INFO,
                "[DB] Profile inserted with id: " + std::to_string(_profileId) +
                    ".",
                __PRETTY_FUNCTION__);
    return {true, std::make_optional<std::uint16_t>(aProfileId)};
  }

  sLog.toFile(LogLevel::WARNING,
              "[DB] Profile insertion failed with id: " +
                  std::to_string(_profileId) + ".",
              __PRETTY_FUNCTION__);
  return {false, std::nullopt};
}

std::pair<bool, std::optional<Profile>>
InMemoryDatabase::get(const std::uint16_t iProfileId) const {
  std::unordered_map<std::uint16_t, std::unique_ptr<Profile>>::const_iterator
      aProfilesIterator{_profiles.find(iProfileId)};

  if (aProfilesIterator != _profiles.end()) {
    return {true, std::make_optional<Profile>(*(aProfilesIterator->second))};
  }

  sLog.toFile(LogLevel::WARNING,
              "[DB] Profile retrieval failed with id: " +
                  std::to_string(_profileId) + ".",
              __PRETTY_FUNCTION__);
  return {false, std::nullopt};
}

bool InMemoryDatabase::remove(const std::uint16_t iProfileId) {
  if (_profiles.count(iProfileId) == 1) {
    _profiles.erase(iProfileId);
    sLog.toFile(LogLevel::INFO,
                "[DB] Profile removed with id: " + std::to_string(_profileId) +
                    ".",
                __PRETTY_FUNCTION__);
    return true;
  }

  sLog.toFile(LogLevel::WARNING,
              "[DB] Profile removal failed with id: " +
                  std::to_string(_profileId) + ".",
              __PRETTY_FUNCTION__);
  return false;
}

bool InMemoryDatabase::update(const std::uint16_t iProfileId,
                              Profile iProfile) {
  if (_profiles.count(iProfileId) == 1) {
    _profiles[iProfileId] = std::make_unique<Profile>(std::move(iProfile));
    sLog.toFile(LogLevel::INFO,
                "[DB] Profile updated with id: " + std::to_string(_profileId) +
                    ".",
                __PRETTY_FUNCTION__);
    return true;
  }

  sLog.toFile(
      LogLevel::WARNING,
      "[DB] Profile update failed with id: " + std::to_string(_profileId) + ".",
      __PRETTY_FUNCTION__);
  return false;
}
} // namespace user