// author: georgiosmatzarapis

#pragma once

#include <optional>

namespace utils {
namespace core_lib {

std::pair<bool, std::optional<std::string>>
ComputeHash(const std::string& iMessage);

} // namespace core_lib
} // namespace utils