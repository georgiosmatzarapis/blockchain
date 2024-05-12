// author: georgiosmatzarapis

#pragma once

#include <chrono>
#include <expected>
#include <optional>

namespace utils {
namespace core_lib {

std::expected<std::string, std::string>
ComputeHash(const std::string& iMessage);

std::expected<bool, std::string> IsHashValid(const std::string& iMessage,
                                             const std::string& iExpectedHash);

std::time_t GetUnixTimestamp(
    const std::optional<std::chrono::system_clock::time_point>& iDatetime =
        std::nullopt);

namespace exception {
class HashCalculationError final : public std::runtime_error {
 public:
  explicit HashCalculationError(const std::string& message);
  const char* what() const noexcept override;
};

class TransactionConsistencyError final : public std::runtime_error {
 public:
  explicit TransactionConsistencyError(const std::string& message);
  const char* what() const noexcept override;
};

class BlockHashCalculationFailure final : public std::runtime_error {
 public:
  explicit BlockHashCalculationFailure(const std::string& message);
  const char* what() const noexcept override;
};
} // namespace exception

} // namespace core_lib
} // namespace utils