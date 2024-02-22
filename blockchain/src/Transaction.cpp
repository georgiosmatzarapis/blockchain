// author: georgiosmatzarapis

#include <iomanip>
#include <iostream>
#include <string>

#include "Transaction.hpp"

namespace transaction {
/* === Helpers === */

std::string RemoveTrailingZeros(const std::string& iAmount) {
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
  std::ostringstream aStream{};
  aStream << std::fixed << std::setprecision(8) << iBitcoinAmount;
  return RemoveTrailingZeros(aStream.str());
}

/* === Coinbase Class === */

Coinbase::Coinbase(std::string owner, const double& bitcoinAmount)
    : _owner{std::move(owner)},
      _bitcoinAmount{bitcoinAmount} {
  _satoshiAmount = BitcoinToSatoshi(_bitcoinAmount);
  _bitcoinRepresentation = BitcoinRepresentation(_bitcoinAmount);
  _timestamp = std::chrono::system_clock::now();
}

Coinbase::Coinbase(const Coinbase& coinbase) = default;

Coinbase& Coinbase::operator=(const Coinbase& coinbase) = default;

Coinbase::Coinbase(Coinbase&& coinbase) noexcept
    : _owner{std::move(coinbase._owner)},
      _bitcoinRepresentation{std::move(coinbase._bitcoinRepresentation)},
      _bitcoinAmount{coinbase._bitcoinAmount},
      _satoshiAmount{coinbase._satoshiAmount},
      _timestamp{coinbase._timestamp} {
  coinbase._bitcoinAmount = 0;
  coinbase._satoshiAmount = 0;
  coinbase._timestamp = std::chrono::system_clock::time_point::min();
}

Coinbase& Coinbase::operator=(Coinbase&& coinbase) noexcept {
  if (this != &coinbase) {
    _owner = std::move(coinbase._owner);
    _bitcoinRepresentation = std::move(coinbase._bitcoinRepresentation);
    _bitcoinAmount = coinbase._bitcoinAmount;
    _satoshiAmount = coinbase._satoshiAmount;
    _timestamp = coinbase._timestamp;
    coinbase._bitcoinAmount = 0;
    coinbase._satoshiAmount = 0;
    coinbase._timestamp = std::chrono::system_clock::time_point::min();
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

std::uint64_t Coinbase::getSatoshiAmount() const { return _satoshiAmount; }

std::string Coinbase::getBitcoinRepresentation() const {
  return _bitcoinRepresentation;
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

} // namespace transaction