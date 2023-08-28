// author: georgiosmatzarapis

#include <iostream>
#include <utility>

#include "../include/Block.hpp"

namespace block {
Block::Block(std::string hash, std::string previousHash, const std::uint32_t& index,
						 std::vector<std::string> data, const std::uint64_t& nonce
) : _hash(std::move(hash)), _previousHash(std::move(previousHash)), _index(index), _data(std::move(data)),
		_nonce(nonce) {}

std::string Block::getHash() const {
	return _hash;
}

std::string Block::getPreviousHash() const {
	return _previousHash;
}

std::uint32_t Block::getIndex() const {
	return _index;
}

std::vector<std::string> Block::getData() const {
	return _data;
}

void Block::display() const {
	std::cout << "Retrieving data...\n";
	for (const auto& data: _data) {
		std::cout << data << "\n";
	}
	std::cout << "\nEnd of data for block [" << _hash << "]\n";
}

}
