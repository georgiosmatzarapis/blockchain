// author: georgiosmatzarapis

#include <iostream>

#include "Block.hpp"
#include "Common.hpp"
#include "Logger.hpp"

namespace block {

using namespace utils;

static const Log& sLog{Log::GetInstance()};

Block::Block() = default;

Block::Block(std::string previousHash, const std::uint32_t& index,
             std::vector<std::unique_ptr<Payload>> payloads,
             std::optional<std::vector<std::unique_ptr<Coinbase>>> coinbases)
    : _previousHash{std::move(previousHash)},
      _index{index},
      _payloads{std::move(payloads)},
      _coinbases{std::move(coinbases)},
      _creationTime{std::chrono::system_clock::now()} {
  calculateMerkleRootHash();
}

Block::Block(std::string previousHash, const std::uint32_t& index,
             std::vector<std::unique_ptr<Coinbase>> coinbases,
             std::optional<std::vector<std::unique_ptr<Payload>>> payloads)
    : _previousHash{std::move(previousHash)},
      _index{index},
      _coinbases{std::move(coinbases)},
      _payloads{std::move(payloads)},
      _creationTime{std::chrono::system_clock::now()} {
  calculateMerkleRootHash();
}

// Public API

std::string Block::getHash() const { return _hash; }

std::string Block::getPreviousHash() const { return _previousHash; }

std::uint32_t Block::getIndex() const { return _index; }

std::string Block::getMerkleRootHash() const { return _merkleRootHash; }

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

// Private API

void Block::groupTransactionHashes() {
  if (_coinbases.has_value()) {
    std::for_each(_coinbases.value().begin(), _coinbases.value().end(),
                  [this](const std::unique_ptr<Coinbase>& iCoinbase) {
                    _transactionHashes.emplace_back(iCoinbase->getHash());
                  });
  }
  if (_payloads.has_value()) {
    std::for_each(_payloads.value().begin(), _payloads.value().end(),
                  [this](const std::unique_ptr<Payload>& iPayload) {
                    _transactionHashes.emplace_back(iPayload->getHash());
                  });
  }
};

void Block::calculateMerkleRootHash() {
  groupTransactionHashes();

  if (_transactionHashes.empty()) {
    const std::string aErrorMessage{
        "No transaction found to calculate the Merkle root hash."};
    sLog.toFile(LogLevel::ERROR, aErrorMessage, __PRETTY_FUNCTION__);
    throw std::runtime_error{aErrorMessage};
  } else if (_transactionHashes.size() == 1) {
    const std::pair<bool, std::optional<std::string>> aHash{
        core_lib::ComputeHash(_transactionHashes[0])};
    if (aHash.first) {
      _merkleRootHash = aHash.second.value();
      return;
    } else {
      const std::string aErrorMessage{"Merkle root hash calculation failed."};
      sLog.toFile(LogLevel::ERROR, aErrorMessage, __PRETTY_FUNCTION__);
      throw std::runtime_error{aErrorMessage};
    }
  } else {
    std::vector<std::string> aMerkleTree{_transactionHashes};

    while (aMerkleTree.size() > 1) {
      std::vector<std::string> aNewLevel{};

      for (std::size_t aMerkleTreeIndex{};
           aMerkleTreeIndex < aMerkleTree.size(); aMerkleTreeIndex += 2) {
        std::string aPair{aMerkleTree[aMerkleTreeIndex]};
        if (aMerkleTreeIndex + 1 < aMerkleTree.size()) {
          aPair += aMerkleTree[aMerkleTreeIndex + 1];
        }

        const std::pair<bool, std::optional<std::string>> aNewHash{
            core_lib::ComputeHash(aPair)};
        std::string aNewHashValue{};
        if (aNewHash.first) {
          aNewHashValue = aNewHash.second.value();
        } else {
          const std::string aErrorMessage{
              "Merkle root hash calculation failed."};
          sLog.toFile(LogLevel::ERROR, aErrorMessage, __PRETTY_FUNCTION__);
          throw std::runtime_error{aErrorMessage};
        }

        aNewLevel.emplace_back(aNewHashValue);
      }

      aMerkleTree = aNewLevel;
    }
    _merkleRootHash = aMerkleTree[0];
    return;
  }
}

} // namespace block