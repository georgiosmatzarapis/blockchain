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
  [[nodiscard]] const std::optional<std::vector<std::unique_ptr<Coinbase>>>&
  getCoinbases() const;
  [[nodiscard]] const std::optional<std::vector<std::unique_ptr<Payload>>>&
  getPayloads() const;

  void display() const;

 private:
  std::uint32_t _index{};
  std::string _merkleRootHash{};
  std::chrono::system_clock::time_point _creationTime{};
  std::uint64_t _nonce{};
  std::string _previousHash{};
  std::string _hash{};
  std::optional<std::vector<std::unique_ptr<Payload>>> _payloads{};
  std::optional<std::vector<std::unique_ptr<Coinbase>>> _coinbases{};
  std::vector<std::string> _transactionHashes{};

  void groupTransactionHashes();
  /**
   * @brief Calculate the Merkle root hash in a simplified way using vector as
   * main data structure.
   */
  void calculateMerkleRootHash();
};
} // namespace block