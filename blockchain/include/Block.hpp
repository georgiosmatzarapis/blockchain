// author: georgiosmatzarapis

#pragma once

#include <cstdint>
#include <string>
#include <vector>

#include "TransactionPayload.hpp"

namespace block {
class Block {
 public:
  Block() = default;
  explicit Block(std::string hash, std::string previousHash,
                 const std::uint32_t& index,
                 std::vector<std::unique_ptr<transaction::Payload>> data,
                 const std::uint64_t& nonce);

  [[nodiscard]] std::string getHash() const;
  [[nodiscard]] std::string getPreviousHash() const;
  [[nodiscard]] std::uint32_t getIndex() const;
  [[nodiscard]] const std::vector<std::unique_ptr<transaction::Payload>>&
  getData() const;

  void display() const;

 private:
  std::uint32_t _index{};
  std::vector<std::uint8_t> _merkleRootHash{};
  std::chrono::system_clock::time_point _creationTime{};
  std::uint64_t _nonce{};
  std::string _previousHash{};
  std::string _hash{};
  std::vector<std::unique_ptr<transaction::Payload>> _data{};
};
} // namespace block