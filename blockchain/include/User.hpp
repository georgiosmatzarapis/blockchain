// author: georgiosmatzarapis

#pragma once

#include <cstdint>
#include <memory>
#include <optional>
#include <string>
#include <unordered_map>

namespace user {
class Profile {
 public:
  explicit Profile(std::string fullName, const std::uint8_t& age,
                   const double& deposit);

  [[nodiscard]] std::string getFullName() const;
  [[nodiscard]] std::uint8_t getAge() const;
  [[nodiscard]] double getDeposit() const;

  /**
   * @brief Update the user deposit.
   * The update will be performed if the final deposit is equal to or greater
   * than zero.
   * @param iAmount Amount to update the deposit with.
   * @return Update status.
   */
  bool updateDeposit(const double iAmount);

 private:
  std::string _fullName{};
  std::uint8_t _age{};
  double _deposit{};
};

class InMemoryDatabase {
 public:
  InMemoryDatabase(const InMemoryDatabase&) = delete;
  InMemoryDatabase& operator=(const InMemoryDatabase&) = delete;
  InMemoryDatabase(InMemoryDatabase&&) noexcept = delete;
  InMemoryDatabase& operator=(InMemoryDatabase&&) noexcept = delete;

  static InMemoryDatabase& GetInstance();

  /**
   * @brief Insert new profile in the database.
   * @param iProfile User profile.
   * @return Insertion status and optionally unique identifier of the record.
   */
  std::pair<bool, std::optional<std::uint16_t>> insert(Profile iProfile);

  /**
   * @brief Retrieve a profile from the database.
   * @param iProfileId Profile id.
   * @return Retrieval status and optionally profile record.
   */
  [[nodiscard]] std::pair<bool, std::optional<Profile>>
  get(const std::uint16_t iProfileId) const;

  /**
   * @brief Remove profile from the database.
   * @param iProfileId Profile id.
   * @return Removal status.
   */
  bool remove(const std::uint16_t iProfileId);

  /**
   * @brief Update profile in the database.
   * @param iProfileId Profile id.
   * @param iProfile User profile.
   * @return Update status.
   */
  bool update(const std::uint16_t iProfileId, Profile iProfile);

 private:
  InMemoryDatabase();
  ~InMemoryDatabase();

  inline std::uint16_t idGenerator() { return ++_profileId; }

  std::int16_t _profileId{-1};
  std::unordered_map<std::uint16_t, std::unique_ptr<Profile>> _profiles{};
};
} // namespace user