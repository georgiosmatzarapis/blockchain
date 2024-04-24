// author: georgiosmatzarapis

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
      _creationTime{std::chrono::system_clock::now()} {
  if (coinbases.has_value()) {
    validateAndStoreTransactions(std::move(coinbases.value()));
  }
  validateAndStoreTransactions(std::move(payloads));
  calculateMerkleRootHash();
}

Block::Block(std::string previousHash, const std::uint32_t& index,
             std::vector<std::unique_ptr<Coinbase>> coinbases,
             std::optional<std::vector<std::unique_ptr<Payload>>> payloads)
    : _previousHash{std::move(previousHash)},
      _index{index},
      _creationTime{std::chrono::system_clock::now()} {
  if (payloads.has_value()) {
    validateAndStoreTransactions(std::move(payloads.value()));
  }
  validateAndStoreTransactions(std::move(coinbases));
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

template <class Transaction>
void Block::validateAndStoreTransactions(
    std::vector<std::unique_ptr<Transaction>>&& ioTransactions) {
  static_assert(std::is_same<Transaction, Coinbase>::value ||
                    std::is_same<Transaction, Payload>::value,
                "Transaction type must be either Coinbase or Payload");

  bool aValidTransactionDetected{false};

  std::for_each(
      ioTransactions.begin(), ioTransactions.end(),
      [this,
       &aValidTransactionDetected](std::unique_ptr<Transaction>& aTransaction) {
        std::string aTempMessageToHash{};
        if constexpr (std::is_same<Transaction, Coinbase>::value) {
          aTempMessageToHash =
              aTransaction->getOwner() +
              std::to_string(aTransaction->getSatoshiAmount()) +
              std::to_string(aTransaction->getUnixTimestamp());
        } else {
          aTempMessageToHash =
              aTransaction->getOwner() + aTransaction->getReceiver() +
              std::to_string(aTransaction->getSatoshiAmount()) +
              std::to_string(aTransaction->getUnixTimestamp());
        }

        const std::string aTempExpectedHash{aTransaction->getHash()};

        const std::expected<bool, std::string> aIsHashValid{
            core_lib::IsHashValid(aTempMessageToHash, aTempExpectedHash)};
        if (aIsHashValid) {
          if (aIsHashValid.value()) {
            aValidTransactionDetected = true;
            if constexpr (std::is_same<Transaction, Coinbase>::value) {
              _coinbases.has_value()
                  ? _coinbases.value().push_back(std::move(aTransaction))
                  : _coinbases.emplace().push_back(std::move(aTransaction));
            } else {
              _payloads.has_value()
                  ? _payloads.value().push_back(std::move(aTransaction))
                  : _payloads.emplace().push_back(std::move(aTransaction));
            }
          } else {
            sLog.toFile(LogLevel::WARNING,
                        "Hash inconsistency detected for message '" +
                            aTempMessageToHash +
                            "', with expected hash: " + aTempExpectedHash,
                        __PRETTY_FUNCTION__);
          }
        } else {
          sLog.toFile(LogLevel::ERROR, aIsHashValid.error(),
                      __PRETTY_FUNCTION__);
          throw core_lib::HashCalculationError{aIsHashValid.error()};
        }
      });

  if (!aValidTransactionDetected) {
    sLog.toFile(LogLevel::WARNING,
                "No valid transaction(s) found to store into the block.",
                __PRETTY_FUNCTION__);
  }
}

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
    const std::expected<std::string, std::string> aHash{
        core_lib::ComputeHash(_transactionHashes[0])};
    if (!aHash) {
      sLog.toFile(LogLevel::ERROR, aHash.error(), __PRETTY_FUNCTION__);
      throw core_lib::HashCalculationError{aHash.error()};
    }
    _merkleRootHash = aHash.value();
    return;
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

        const std::expected<std::string, std::string> aNewHash{
            core_lib::ComputeHash(aPair)};
        if (!aNewHash) {
          sLog.toFile(LogLevel::ERROR, aNewHash.error(), __PRETTY_FUNCTION__);
          throw core_lib::HashCalculationError{aNewHash.error()};
        }
        aNewLevel.emplace_back(aNewHash.value());
      }

      aMerkleTree = aNewLevel;
    }
    _merkleRootHash = aMerkleTree[0];
    return;
  }
}

} // namespace block