// author: georgiosmatzarapis

#pragma once

#include <cstdint>
#include <ctime>
#include <string>
#include <vector>

namespace block {
class Block {
	public:
		Block(std::string hash, std::string previousHash, const std::uint32_t& index,
					std::vector<std::string> data, const std::uint64_t& nonce);
		// TODO Think for Rule of Five guideline

		[[nodiscard]] std::string getHash() const;
		[[nodiscard]] std::string getPreviousHash() const;
		[[nodiscard]] std::uint32_t getIndex() const;
		[[nodiscard]] std::vector<std::string> getData() const;

		void display() const;

	private:
		std::uint32_t _index;
		std::vector<uint8_t> _merkleRootHash;
		std::time_t _creationTime;
		std::uint64_t _nonce;
		std::string _previousHash;
		std::string _hash;
		std::vector<std::string> _data;
};
} // namespace block
