// author: georgiosmatzarapis

#pragma once

#include <chrono>
#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

#include "TransactionPayload.hpp"

namespace block {
class Block {
 public:
  Block(std::string hash, std::string previousHash, const std::uint32_t& index,
        std::unique_ptr<std::vector<transaction::Payload>> data,
        const std::uint64_t& nonce);

  [[nodiscard]] std::string getHash() const;
  [[nodiscard]] std::string getPreviousHash() const;
  [[nodiscard]] std::uint32_t getIndex() const;
  [[nodiscard]] const std::unique_ptr<std::vector<transaction::Payload>>&
  getData() const;

  void display() const;

 private:
  std::uint32_t _index{};
  std::vector<std::uint8_t> _merkleRootHash{};
  std::chrono::system_clock::time_point _creationTime{};
  std::uint64_t _nonce{};
  std::string _previousHash{};
  std::string _hash{};
  std::unique_ptr<std::vector<transaction::Payload>> _data{};
};
} // namespace block
