// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "../blockchain/include/Block.hpp"

class BlockTest : public ::testing::Test {
 protected:
  BlockTest()
      : _testData{{"hash", std::string{"0#8JNOINQ721"}},
                  {"previousHash", std::string{"0#981OHHH721"}},
                  {"index", static_cast<std::uint32_t>(1)},
                  {"data", transaction::Payload{"Owner", "Receiver", 1.2}},
                  {"nonce", static_cast<std::uint64_t>(1)}} {
    _payloadData.push_back(std::make_unique<transaction::Payload>(
        std::get<transaction::Payload>(_testData["data"])));
    _block = block::Block{std::get<std::string>(_testData["hash"]),
                          std::get<std::string>(_testData["previousHash"]),
                          std::get<std::uint32_t>(_testData["index"]),
                          std::move(_payloadData),
                          std::get<std::uint64_t>(_testData["nonce"])};
  }

  std::map<std::string, std::variant<std::string, std::uint32_t, std::uint64_t,
                                     transaction::Payload>>
      _testData{};
  std::vector<std::unique_ptr<transaction::Payload>> _payloadData{};
  block::Block _block{};
};

TEST_F(BlockTest, ShouldReturnHash) {
  EXPECT_EQ(_block.getHash(), std::get<std::string>(_testData["hash"]));
}

TEST_F(BlockTest, ShouldReturnPreviousHash) {
  EXPECT_EQ(_block.getPreviousHash(),
            std::get<std::string>(_testData["previousHash"]));
}

TEST_F(BlockTest, ShouldReturnIndex) {
  EXPECT_EQ(_block.getIndex(), std::get<std::uint32_t>(_testData["index"]));
}

TEST_F(BlockTest, ShouldReturnData) {
  const std::vector<std::unique_ptr<transaction::Payload>>& aData{
      _block.getData()};
  EXPECT_EQ(aData[0]->getOwner(),
            std::get<transaction::Payload>(_testData["data"]).getOwner());
  EXPECT_EQ(aData[0]->getReceiver(),
            std::get<transaction::Payload>(_testData["data"]).getReceiver());
  EXPECT_EQ(
      aData[0]->getBitcoinAmount(),
      std::get<transaction::Payload>(_testData["data"]).getBitcoinAmount());
  EXPECT_EQ(aData[0]->getBitcoinRepresentation(),
            std::get<transaction::Payload>(_testData["data"])
                .getBitcoinRepresentation());
  EXPECT_EQ(
      aData[0]->getSatoshiAmount(),
      std::get<transaction::Payload>(_testData["data"]).getSatoshiAmount());
  EXPECT_EQ(aData[0]->getTimestamp(),
            std::get<transaction::Payload>(_testData["data"]).getTimestamp());
}