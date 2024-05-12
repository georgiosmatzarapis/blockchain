// author: georgiosmatzarapis

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "Transaction.hpp"

namespace block {

using namespace transaction;

class Block {
 public:
  Block();
  explicit Block(std::string previousHash, const std::uint32_t& index,
                 std::vector<std::unique_ptr<Payload>> payloads,
                 std::optional<std::vector<std::unique_ptr<Coinbase>>>
                     coinbases = std::nullopt);
  explicit Block(std::string previousHash, const std::uint32_t& index,
                 std::vector<std::unique_ptr<Coinbase>> coinbases,
                 std::optional<std::vector<std::unique_ptr<Payload>>> payloads =
                     std::nullopt);

  [[nodiscard]] std::string getHash() const;
  [[nodiscard]] std::string getPreviousHash() const;
  [[nodiscard]] std::uint32_t getIndex() const;
  [[nodiscard]] std::string getMerkleRootHash() const;
  [[nodiscard]] std::uint32_t getNonce() const;
  [[nodiscard]] std::time_t getCreationTime() const;
  [[nodiscard]] const std::optional<std::vector<std::unique_ptr<Coinbase>>>&
  getCoinbases() const;
  [[nodiscard]] const std::optional<std::vector<std::unique_ptr<Payload>>>&
  getPayloads() const;

  void display() const;

 private:
  std::uint32_t _index{};
  std::string _merkleRootHash{};
  std::time_t _creationTime{};
  std::uint32_t _nonce{};
  std::string _previousHash{};
  std::string _hash{};
  std::optional<std::vector<std::unique_ptr<Payload>>> _payloads{};
  std::optional<std::vector<std::unique_ptr<Coinbase>>> _coinbases{};
  std::vector<std::string> _transactionHashes{};

  static constexpr std::string kTargetDifficulty{"00"};

  void initialize(
      std::optional<std::vector<std::unique_ptr<Coinbase>>>&& ioCoinbases,
      std::optional<std::vector<std::unique_ptr<Payload>>>&& ioPayloads);
  /**
   * @brief Validate the hash of each incoming transaction and store it.
   * @param ioTransactions Transaction type. Can be either Coinbase or Payload.
   * @throw HashCalculationError.
   */
  template <class Transaction>
  void validateAndStoreTransactions(
      std::vector<std::unique_ptr<Transaction>>&& ioTransactions);
  /**
   * @brief Group into a single vector the valid transaction hashes.
   * @throw TransactionConsistencyError.
   */
  void groupTransactionHashes();
  /**
   * @brief Calculate the Merkle root hash in a simplified way using vector as
   * main data structure.
   * @throw HashCalculationError.
   */
  void calculateMerkleRootHash();
  /**
   * @brief Calculate the Block's hash.
   * @throw HashCalculationError, BlockHashCalculationFailure.
   */
  void calculateBlockHash();
};
} // namespace block