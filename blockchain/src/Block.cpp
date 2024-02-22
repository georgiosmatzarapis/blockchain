// author: georgiosmatzarapis

#include <iostream>

#include "Block.hpp"

namespace block {

Block::Block() = default;

Block::Block(std::string previousHash, const std::uint32_t& index,
             std::vector<std::unique_ptr<Payload>> payloads,
             std::optional<std::vector<std::unique_ptr<Coinbase>>> coinbases)
    : _previousHash{std::move(previousHash)},
      _index{index},
      _payloads{std::move(payloads)},
      _coinbases{std::move(coinbases)} {
  _creationTime = std::chrono::system_clock::now();
}

Block::Block(std::string previousHash, const std::uint32_t& index,
             std::vector<std::unique_ptr<Coinbase>> coinbases,
             std::optional<std::vector<std::unique_ptr<Payload>>> payloads)
    : _previousHash{std::move(previousHash)},
      _index{index},
      _coinbases{std::move(coinbases)},
      _payloads{std::move(payloads)} {
  _creationTime = std::chrono::system_clock::now();
}

// Public API

std::string Block::getHash() const { return _hash; }

std::string Block::getPreviousHash() const { return _previousHash; }

std::uint32_t Block::getIndex() const { return _index; }

const std::optional<std::vector<std::unique_ptr<Payload>>>&
Block::getPayloads() const {
  return _payloads;
}

const std::optional<std::vector<std::unique_ptr<Coinbase>>>&
Block::getCoinbases() const {
  return _coinbases;
}

void Block::display() const {
  std::cout << "\nTransactions:\n" << std::endl;
  std::uint8_t aPayloadCounter{}, aCoinbaseCounter{};

  if (_coinbases.has_value()) {
    std::for_each(
        _coinbases.value().begin(), _coinbases.value().end(),
        [&aCoinbaseCounter](const std::unique_ptr<Coinbase>& iCoinbase) {
          std::cout << "[Coinbase#" << static_cast<int>(aCoinbaseCounter) << "]"
                    << std::endl;
          std::cout << "Owner: " << iCoinbase->getOwner() << std::endl;
          std::cout << "Amount in Satoshi: " << iCoinbase->getSatoshiAmount()
                    << std::endl;
          std::cout << "Amount in Bitcoin: "
                    << iCoinbase->getBitcoinRepresentation() << std::endl;
          std::cout << "Coinbase creation: " << iCoinbase->getTimestamp()
                    << std::endl;
          ++aCoinbaseCounter;
          std::cout << "====" << std::endl;
        });
  } else {
    std::cout << "No coinbases found.\n" << std::endl;
  }

  if (_payloads.has_value()) {
    std::for_each(
        _payloads.value().begin(), _payloads.value().end(),
        [&aPayloadCounter](const std::unique_ptr<Payload>& iPayload) {
          std::cout << "[Payload#" << static_cast<int>(aPayloadCounter) << "]"
                    << std::endl;
          std::cout << "Owner: " << iPayload->getOwner() << std::endl;
          std::cout << "Receiver: " << iPayload->getReceiver() << std::endl;
          std::cout << "Amount in Satoshi: " << iPayload->getSatoshiAmount()
                    << std::endl;
          std::cout << "Amount in Bitcoin: "
                    << iPayload->getBitcoinRepresentation() << std::endl;
          std::cout << "Payload creation: " << iPayload->getTimestamp()
                    << std::endl;
          ++aPayloadCounter;
          std::cout << "====";
        });
  } else {
    std::cout << "No payloads found." << std::endl;
  }

  std::cout << "\nEnd of transactions for block#" << _hash << ".\n";
}
} // namespace block