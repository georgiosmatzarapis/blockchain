// author: georgiosmatzarapis

#include <cstring>
#include <memory>

#include "Common.hpp"
#include "Hmac.hpp"
#include "Logger.hpp"
#include "OpenSslApi.hpp"

namespace utils {
namespace core_lib {

std::expected<std::string, std::string>
ComputeHash(const std::string& iMessage) {
  auto sMessage{reinterpret_cast<const unsigned char*>(iMessage.c_str())};
  unsigned char* sDigest{};
  unsigned int sDigestSize{};
  utils::HmacIt(sMessage, std::strlen(reinterpret_cast<const char*>(sMessage)),
                &sDigest, &sDigestSize, std::make_unique<openssl::Api>());

  if (sDigest) {
    Log::GetInstance().toFile(LogLevel::DEBUG,
                              "Hash calculated for message: " + iMessage,
                              __PRETTY_FUNCTION__);
    const std::string sResult(reinterpret_cast<char*>(sDigest), sDigestSize);
    sDigest = nullptr;
    return sResult;
  } else {
    const std::string sErrorMessage{"Hash calculation failed for message: " +
                                    iMessage};
    Log::GetInstance().toFile(LogLevel::ERROR, sErrorMessage,
                              __PRETTY_FUNCTION__);
    return std::unexpected{sErrorMessage};
  }
}

std::expected<bool, std::string> IsHashValid(const std::string& iMessage,
                                             const std::string& iExpectedHash) {
  const std::expected<std::string, std::string> sActualHash{
      ComputeHash(iMessage)};
  if (!sActualHash) {
    return std::unexpected{sActualHash.error()};
  }
  return (sActualHash.value() == iExpectedHash) ? true : false;
}

// HashCalculationError

HashCalculationError::HashCalculationError(const std::string& message)
    : std::runtime_error{message} {}

const char* HashCalculationError::what() const noexcept {
  return std::runtime_error::what();
}
} // namespace core_lib
} // namespace utils