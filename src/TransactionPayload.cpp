// author: georgiosmatzarapis

#include <iomanip>
#include <iostream>
#include <string>

#include "../include/TransactionPayload.hpp"

namespace transaction {

Payload::Payload(std::string sender, std::string receiver, const double& amount,
                 const std::chrono::system_clock::time_point& timestamp)
    : _sender(std::move(sender)),
      _receiver(std::move(receiver)),
      _bitcoinAmount(amount),
      _timestamp(timestamp) {
  _satoshiAmount = bitcoinToSatoshi();
  _bitcoinRepresentation = bitcoinRepresentation();
}

Payload::Payload(transaction::Payload&& payload) noexcept
    : _sender(std::move(payload._sender)),
      _receiver(std::move(payload._receiver)),
      _bitcoinRepresentation(std::move(payload._bitcoinRepresentation)) {
  payload._satoshiAmount = 0;
  payload._bitcoinAmount = 0;
  payload._timestamp = std::chrono::system_clock::time_point::min();
}

Payload::Payload(const Payload& payload) = default;

// Public API

std::string Payload::getSender() const { return _sender; }

std::string Payload::getReceiver() const { return _receiver; }

std::uint64_t Payload::getSatoshiAmount() const { return _satoshiAmount; }

double Payload::getBitcoinAmount() const { return _bitcoinAmount; }

const std::chrono::system_clock::time_point& Payload::getTimestamp() const {
  return _timestamp;
}

std::string Payload::getBitcoinRepresentation() const {
  return _bitcoinRepresentation;
}

// Private API

std::uint64_t Payload::bitcoinToSatoshi() const {
  return static_cast<std::uint64_t>(_bitcoinAmount * 1e8);
}

std::string Payload::bitcoinRepresentation() const {
  std::ostringstream aStream;
  aStream << std::fixed << std::setprecision(8) << _bitcoinAmount;
  return RemoveTrailingZeros(aStream.str());
}

// Static methods

const std::string Payload::RemoveTrailingZeros(const std::string& iAmount) {
  auto sLastNonZero = iAmount.find_last_not_of('0');

  if(sLastNonZero != std::string::npos && iAmount[sLastNonZero] == '.') {
    sLastNonZero = iAmount.find_last_not_of('0', sLastNonZero - 1);
  }

  std::string sTrailedAmount{};
  sTrailedAmount = iAmount.substr(0, sLastNonZero + 1);
  return sTrailedAmount.empty() ? std::string("0") : sTrailedAmount;
}

} // namespace transaction
