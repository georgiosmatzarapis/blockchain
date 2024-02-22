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
  [[nodiscard]] const std::optional<std::vector<std::unique_ptr<Payload>>>&
  getPayloads() const;
  [[nodiscard]] const std::optional<std::vector<std::unique_ptr<Coinbase>>>&
  getCoinbases() const;

  void display() const;

 private:
  std::uint32_t _index{};
  std::vector<std::uint8_t> _merkleRootHash{};
  std::chrono::system_clock::time_point _creationTime{};
  std::uint64_t _nonce{};
  std::string _previousHash{};
  std::string _hash{};
  std::optional<std::vector<std::unique_ptr<Payload>>> _payloads{};
  std::optional<std::vector<std::unique_ptr<Coinbase>>> _coinbases{};
};
} // namespace block