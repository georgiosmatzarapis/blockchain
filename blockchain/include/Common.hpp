// author: georgiosmatzarapis

#pragma once

#include <expected>
#include <optional>

namespace utils {
namespace core_lib {

std::expected<std::string, std::string>
ComputeHash(const std::string& iMessage);

std::expected<bool, std::string> IsHashValid(const std::string& iMessage,
                                             const std::string& iExpectedHash);

class HashCalculationError final : public std::runtime_error {
 public:
  explicit HashCalculationError(const std::string& message);
  const char* what() const noexcept override;
};

} // namespace core_lib
} // namespace utils