// author: georgiosmatzarapis

#include <iostream>
#include <utility>

#include "../include/Block.hpp"

namespace block {
Block::Block(std::string hash, std::string previousHash,
             const std::uint32_t& index,
             std::unique_ptr<std::vector<transaction::Payload>> data,
             const std::uint64_t& nonce)
    : _hash(std::move(hash)),
      _previousHash(std::move(previousHash)),
      _index(index),
      _data(std::move(data)),
      _nonce(nonce) {}

std::string Block::getHash() const { return _hash; }

std::string Block::getPreviousHash() const { return _previousHash; }

std::uint32_t Block::getIndex() const { return _index; }

const std::unique_ptr<std::vector<transaction::Payload>>&
Block::getData() const {
  return _data;
}

void Block::display() const {
  std::cout << "Retrieving data...\n";
  if (_data) {
    for (const transaction::Payload& payload : *_data) {
      std::cout << payload.getSender() << std::endl;
      std::cout << payload.getReceiver() << std::endl;
      std::cout << payload.getSatoshiAmount() << std::endl;
      std::cout << payload.getBitcoinRepresentation() << std::endl;
      std::cout << payload.getTimestamp() << std::endl;
      std::cout << "---";
    }
  } else {
    std::cout << "Data is empty." << std::endl;
  }

  std::cout << "\nEnd of data for block [" << _hash << "]\n";
}

} // namespace block
