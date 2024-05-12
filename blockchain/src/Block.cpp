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
      _creationTime{core_lib::GetUnixTimestamp()} {
  initialize(std::move(coinbases), std::make_optional(std::move(payloads)));
}

Block::Block(std::string previousHash, const std::uint32_t& index,
             std::vector<std::unique_ptr<Coinbase>> coinbases,
             std::optional<std::vector<std::unique_ptr<Payload>>> payloads)
    : _previousHash{std::move(previousHash)},
      _index{index},
      _creationTime{core_lib::GetUnixTimestamp()} {
  initialize(std::make_optional(std::move(coinbases)), std::move(payloads));
}

// Public API

std::string Block::getHash() const { return _hash; }

std::string Block::getPreviousHash() const { return _previousHash; }

std::uint32_t Block::getIndex() const { return _index; }

std::string Block::getMerkleRootHash() const { return _merkleRootHash; }

std::uint32_t Block::getNonce() const { return _nonce; }

std::time_t Block::getCreationTime() const { return _creationTime; }

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

  std::cout << "\nEnd of transactions for block with hash: " << _hash
            << std::endl;
}

// Private API

void Block::initialize(
    std::optional<std::vector<std::unique_ptr<Coinbase>>>&& ioCoinbases,
    std::optional<std::vector<std::unique_ptr<Payload>>>&& ioPayloads) {
  if (ioCoinbases.has_value()) {
    validateAndStoreTransactions(std::move(ioCoinbases.value()));
  }
  if (ioPayloads.has_value()) {
    validateAndStoreTransactions(std::move(ioPayloads.value()));
  }
  groupTransactionHashes();
  calculateMerkleRootHash();
  calculateBlockHash();
}

template <class Transaction>
void Block::validateAndStoreTransactions(
    std::vector<std::unique_ptr<Transaction>>&& ioTransactions) {
  static_assert(std::is_same<Transaction, Coinbase>::value ||
                    std::is_same<Transaction, Payload>::value,
                "Transaction type must be either Coinbase or Payload");

  std::for_each(
      ioTransactions.begin(), ioTransactions.end(),
      [this](std::unique_ptr<Transaction>& aTransaction) {
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
          throw core_lib::exception::HashCalculationError{aIsHashValid.error()};
        }
      });
}

void Block::groupTransactionHashes() {
  bool aValidTransactionExist{false};
  if (_coinbases.has_value()) {
    std::for_each(_coinbases.value().begin(), _coinbases.value().end(),
                  [this](const std::unique_ptr<Coinbase>& iCoinbase) {
                    _transactionHashes.emplace_back(iCoinbase->getHash());
                  });
    aValidTransactionExist = true;
  }
  if (_payloads.has_value()) {
    std::for_each(_payloads.value().begin(), _payloads.value().end(),
                  [this](const std::unique_ptr<Payload>& iPayload) {
                    _transactionHashes.emplace_back(iPayload->getHash());
                  });
    aValidTransactionExist = true;
  }
  if (!aValidTransactionExist) {
    const std::string aErrorMessage{"No valid transaction(s) found to store."};
    sLog.toFile(LogLevel::ERROR, aErrorMessage, __PRETTY_FUNCTION__);
    throw core_lib::exception::TransactionConsistencyError(aErrorMessage);
  }
};

void Block::calculateMerkleRootHash() {
  if (_transactionHashes.size() == 1) {
    const std::expected<std::string, std::string> aHash{
        core_lib::ComputeHash(_transactionHashes[0])};
    if (!aHash) {
      sLog.toFile(LogLevel::ERROR, aHash.error(), __PRETTY_FUNCTION__);
      throw core_lib::exception::HashCalculationError{aHash.error()};
    }
    _merkleRootHash = aHash.value();
    return;
  }

  std::vector<std::string> aMerkleTree{_transactionHashes};
  while (aMerkleTree.size() > 1) {
    std::vector<std::string> aNewLevel{};

    for (std::size_t aMerkleTreeIndex{}; aMerkleTreeIndex < aMerkleTree.size();
         aMerkleTreeIndex += 2) {
      std::string aPair{aMerkleTree[aMerkleTreeIndex]};
      if (aMerkleTreeIndex + 1 < aMerkleTree.size()) {
        aPair += aMerkleTree[aMerkleTreeIndex + 1];
      }

      const std::expected<std::string, std::string> aNewHash{
          core_lib::ComputeHash(aPair)};
      if (!aNewHash) {
        sLog.toFile(LogLevel::ERROR, aNewHash.error(), __PRETTY_FUNCTION__);
        throw core_lib::exception::HashCalculationError{aNewHash.error()};
      }
      aNewLevel.emplace_back(aNewHash.value());
    }

    aMerkleTree = aNewLevel;
  }
  _merkleRootHash = aMerkleTree[0];
  return;
}

void Block::calculateBlockHash() {
  const std::string aHeader{std::to_string(_index) + _previousHash +
                            _merkleRootHash + std::to_string(_creationTime)};
  for (_nonce = 0; _nonce < 1000000; ++_nonce) {
    const std::expected<std::string, std::string> aHash{
        core_lib::ComputeHash(aHeader + std::to_string(_nonce))};
    if (!aHash) {
      sLog.toFile(LogLevel::ERROR, aHash.error(), __PRETTY_FUNCTION__);
      throw core_lib::exception::HashCalculationError(aHash.error());
    }
#ifndef NDEBUG
    _hash = aHash.value();
    return;
#else
    if (aHash.value().substr(0, 2) == kTargetDifficulty) {
      _hash = aHash.value();
      return;
    }
#endif
  }
  const std::string aErrorMessage{
      "Block hash calculation failed for target difficulty: " +
      kTargetDifficulty};
  sLog.toFile(LogLevel::ERROR, aErrorMessage, __PRETTY_FUNCTION__);
  throw core_lib::exception::BlockHashCalculationFailure{aErrorMessage};
}
} // namespace block