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
    /**
     * Commented out as it generates massive amount of logs while calculating a
     * block's hash in Release mode.
     * @todo (#TODO) Implement the correlation of the log level with the build
     * variant.
     */
    // Log::GetInstance().toFile(LogLevel::DEBUG,
    //                           "Hash calculated for message: " + iMessage,
    //                           __PRETTY_FUNCTION__);
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

std::time_t GetUnixTimestamp(
    const std::optional<std::chrono::system_clock::time_point>& iDatetime) {
  const auto sTimePoint{iDatetime.has_value()
                            ? iDatetime.value()
                            : std::chrono::system_clock::now()};
  const auto sDurationSinceEpoch{sTimePoint.time_since_epoch()};
  return std::chrono::duration_cast<std::chrono::seconds>(sDurationSinceEpoch)
      .count();
}

namespace exception {
// HashCalculationError

HashCalculationError::HashCalculationError(const std::string& message)
    : std::runtime_error{message} {}

const char* HashCalculationError::what() const noexcept {
  return std::runtime_error::what();
}

// TransactionConsistencyError

TransactionConsistencyError::TransactionConsistencyError(
    const std::string& message)
    : std::runtime_error{message} {}

const char* TransactionConsistencyError::what() const noexcept {
  return std::runtime_error::what();
}

// BlockHashCalculationFailure

BlockHashCalculationFailure::BlockHashCalculationFailure(
    const std::string& message)
    : std::runtime_error{message} {}

const char* BlockHashCalculationFailure::what() const noexcept {
  return std::runtime_error::what();
}
} // namespace exception

} // namespace core_lib
} // namespace utils