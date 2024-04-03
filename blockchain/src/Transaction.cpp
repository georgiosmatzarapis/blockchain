// author: georgiosmatzarapis

#include <iomanip>
#include <iostream>

#include "Common.hpp"
#include "Transaction.hpp"

namespace transaction {

static constexpr std::time_t kDefaultUnixTimestamp{946684800};

/* === Helpers === */

static std::string RemoveTrailingZeros(const std::string& iAmount) {
  std::size_t sLastNonZero{iAmount.find_last_not_of('0')};

  if (sLastNonZero != std::string::npos && iAmount[sLastNonZero] == '.') {
    sLastNonZero = iAmount.find_last_not_of('0', sLastNonZero - 1);
  }

  std::string sTrailedAmount{iAmount.substr(0, sLastNonZero + 1)};
  return sTrailedAmount.empty() ? std::string("0") : sTrailedAmount;
}

std::uint64_t BitcoinToSatoshi(const double& iBitcoinAmount) {
  return static_cast<std::uint64_t>(iBitcoinAmount * 1e8);
}

std::string BitcoinRepresentation(const double& iBitcoinAmount) {
  std::ostringstream sStream{};
  sStream << std::fixed << std::setprecision(8) << iBitcoinAmount;
  return RemoveTrailingZeros(sStream.str());
}

/* === Coinbase Class === */

Coinbase::Coinbase(std::string owner, const double& bitcoinAmount)
    : _owner{std::move(owner)},
      _bitcoinAmount{bitcoinAmount},
      _satoshiAmount{BitcoinToSatoshi(_bitcoinAmount)},
      _bitcoinRepresentation{BitcoinRepresentation(_bitcoinAmount)},
      _timestamp{std::chrono::system_clock::now()} {
  calculateUnixTimestamp();
}

Coinbase::Coinbase(const Coinbase& coinbase) = default;

Coinbase& Coinbase::operator=(const Coinbase& coinbase) = default;

Coinbase::Coinbase(Coinbase&& coinbase) noexcept
    : _owner{std::move(coinbase._owner)},
      _bitcoinRepresentation{std::move(coinbase._bitcoinRepresentation)},
      _hash{std::move(coinbase._hash)},
      _bitcoinAmount{coinbase._bitcoinAmount},
      _satoshiAmount{coinbase._satoshiAmount},
      _timestamp{coinbase._timestamp},
      _unixTimestamp{coinbase._unixTimestamp} {
  coinbase._bitcoinAmount = 0;
  coinbase._satoshiAmount = 0;
  coinbase._timestamp = std::chrono::system_clock::time_point::min();
  coinbase._unixTimestamp = kDefaultUnixTimestamp;
}

Coinbase& Coinbase::operator=(Coinbase&& coinbase) noexcept {
  if (this != &coinbase) {
    _owner = std::move(coinbase._owner);
    _bitcoinRepresentation = std::move(coinbase._bitcoinRepresentation);
    _hash = std::move(coinbase._hash);
    _bitcoinAmount = coinbase._bitcoinAmount;
    _satoshiAmount = coinbase._satoshiAmount;
    _timestamp = coinbase._timestamp;
    _unixTimestamp = coinbase._unixTimestamp;
    coinbase._bitcoinAmount = 0;
    coinbase._satoshiAmount = 0;
    coinbase._timestamp = std::chrono::system_clock::time_point::min();
    coinbase._unixTimestamp = kDefaultUnixTimestamp;
  }
  return *this;
}

Coinbase::~Coinbase() = default;

// Public API

std::string Coinbase::getOwner() const { return _owner; }

double Coinbase::getBitcoinAmount() const { return _bitcoinAmount; }

std::chrono::system_clock::time_point Coinbase::getTimestamp() const {
  return _timestamp;
}

std::time_t Coinbase::getUnixTimestamp() const { return _unixTimestamp; }

std::uint64_t Coinbase::getSatoshiAmount() const { return _satoshiAmount; }

std::string Coinbase::getBitcoinRepresentation() const {
  return _bitcoinRepresentation;
}

std::string Coinbase::getHash() {
  if (!_hash.size()) {
    const auto aSatoshiAmountCppStr{std::to_string(_satoshiAmount)};
    const auto aUnixTimestampCppStr{std::to_string(_unixTimestamp)};
    const std::string aMessage{_owner + aSatoshiAmountCppStr +
                               aUnixTimestampCppStr};
    const std::pair<bool, std::optional<std::string>> aHash{
        utils::core_lib::ComputeHash(aMessage)};

    if (aHash.first) {
      _hash = aHash.second.value();
    } else {
      throw std::runtime_error{
          "Error while computing the Coinbase hash for message '" + aMessage +
          "'."};
    }
  }
  return _hash;
}

// Private API

void Coinbase::calculateUnixTimestamp() {
  const auto aDurationSinceEpoch{_timestamp.time_since_epoch()};
  const auto aSeconds{
      std::chrono::duration_cast<std::chrono::seconds>(aDurationSinceEpoch)};
  _unixTimestamp = aSeconds.count();
}

/* === Payload Class === */

Payload::Payload(std::string owner, std::string receiver, const double& amount)
    : Coinbase{owner, amount},
      _receiver{std::move(receiver)} {}

Payload::Payload(const Payload& payload) = default;

Payload& Payload::operator=(const Payload& payload) = default;

Payload::Payload(Payload&& payload) noexcept
    : Coinbase{std::move(payload)},
      _receiver{std::move(payload._receiver)} {}

Payload& Payload::operator=(Payload&& payload) noexcept {
  if (this != &payload) {
    Coinbase::operator=(std::move(payload));
    _receiver = std::move(payload._receiver);
  }
  return *this;
}

Payload::~Payload() = default;

// Public API

std::string Payload::getReceiver() const { return _receiver; }

std::string Payload::getHash() {
  if (!_hash.size()) {
    const auto aSatoshiAmountCppStr{std::to_string(getSatoshiAmount())};
    const auto aUnixTimestampCppStr{std::to_string(getUnixTimestamp())};
    const std::string aMessage{getOwner() + _receiver + aSatoshiAmountCppStr +
                               aUnixTimestampCppStr};
    std::pair<bool, std::optional<std::string>> aComputedHash{
        utils::core_lib::ComputeHash(aMessage)};

    if (aComputedHash.first) {
      _hash = aComputedHash.second.value();
    } else {
      throw std::runtime_error{
          "Error while computing the Payload hash for message '" + aMessage +
          "'."};
    }
  }
  return _hash;
}

} // namespace transaction