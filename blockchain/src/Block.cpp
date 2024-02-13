// author: georgiosmatzarapis

#include <iostream>

#include "Block.hpp"

namespace block {
Block::Block(std::string hash, std::string previousHash,
             const std::uint32_t& index,
             std::vector<std::unique_ptr<transaction::Payload>> data,
             const std::uint64_t& nonce)
    : _hash{std::move(hash)},
      _previousHash{std::move(previousHash)},
      _index{index},
      _data{std::move(data)},
      _nonce{nonce} {
  _creationTime = std::chrono::system_clock::now();
}

std::string Block::getHash() const { return _hash; }

std::string Block::getPreviousHash() const { return _previousHash; }

std::uint32_t Block::getIndex() const { return _index; }

const std::vector<std::unique_ptr<transaction::Payload>>&
Block::getData() const {
  return _data;
}

void Block::display() const {
  std::cout << "\nBlock data:\n" << std::endl;
  std::uint8_t aPayloadCounter{};
  if (_data.size()) {
    std::for_each(
        _data.begin(), _data.end(),
        [&aPayloadCounter](
            const std::unique_ptr<transaction::Payload>& iPayload) {
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
    std::cout << "Data is empty." << std::endl;
  }
  std::cout << "\nEnd of data for block [#" << _hash << "]\n";
}

} // namespace block