// author: georgiosmatzarapis

#include <cstring>
#include <memory>

#include "Common.hpp"
#include "Hmac.hpp"
#include "Logger.hpp"
#include "OpenSslApi.hpp"

namespace utils {
namespace core_lib {
std::pair<bool, std::optional<std::string>>
ComputeHash(const std::string& iMessage) {
  auto sMessage{reinterpret_cast<const unsigned char*>(iMessage.c_str())};
  unsigned char* sDigest{};
  unsigned int sDigestSize{};
  utils::HmacIt(sMessage, std::strlen(reinterpret_cast<const char*>(sMessage)),
                &sDigest, &sDigestSize, std::make_unique<openssl::Api>());

  if (sDigest) {
    Log::GetInstance().toFile(LogLevel::INFO,
                              "Hash calculated for message '" + iMessage + "'.",
                              __PRETTY_FUNCTION__);
    const std::string sResult(reinterpret_cast<char*>(sDigest), sDigestSize);
    sDigest = nullptr;
    return {true, std::make_optional<std::string>(sResult)};
  } else {
    Log::GetInstance().toFile(LogLevel::ERROR,
                              "Hash calculation failed for message '" +
                                  iMessage + "'.",
                              __PRETTY_FUNCTION__);
    return {false, std::nullopt};
  }
}
} // namespace core_lib
} // namespace utils