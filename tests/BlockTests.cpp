// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "Block.hpp"

namespace block {
namespace tests {

class BlockTest : public ::testing::Test {
 protected:
  BlockTest()
      : _testData{{"previousHash", std::string{"0#981OHHH721"}},
                  {"index", static_cast<std::uint32_t>(1)},
                  {"payload", Payload{"Owner", "Receiver", 1.2}},
                  {"coinbase", Coinbase{"Owner", 1.2}}} {
    _payloads.push_back(
        std::make_unique<Payload>(std::get<Payload>(_testData["payload"])));
    _coinbases.push_back(
        std::make_unique<Coinbase>(std::get<Coinbase>(_testData["coinbase"])));
    _payloadBlock = Block{std::get<std::string>(_testData["previousHash"]),
                          std::get<std::uint32_t>(_testData["index"]),
                          std::move(_payloads)};
    _coinbaseBlock = Block{std::get<std::string>(_testData["previousHash"]),
                           std::get<std::uint32_t>(_testData["index"]),
                           std::move(_coinbases)};
    _fullBlock = Block{std::get<std::string>(_testData["previousHash"]),
                       std::get<std::uint32_t>(_testData["index"]),
                       std::move(_coinbases), std::move(_payloads)};
  }

  std::map<std::string, std::variant<std::string, std::uint32_t, std::uint64_t,
                                     Payload, Coinbase>>
      _testData{};
  std::vector<std::unique_ptr<Coinbase>> _coinbases{};
  std::vector<std::unique_ptr<Payload>> _payloads{};
  Block _payloadBlock{};
  Block _coinbaseBlock{};
  Block _fullBlock{};
};

TEST_F(BlockTest, ShouldReturnHash) {
  ASSERT_EQ(_payloadBlock.getHash(), std::get<std::string>(_testData["hash"]));
}

TEST_F(BlockTest, ShouldReturnPreviousHash) {
  ASSERT_EQ(_payloadBlock.getPreviousHash(),
            std::get<std::string>(_testData["previousHash"]));
}

TEST_F(BlockTest, ShouldReturnIndex) {
  ASSERT_EQ(_payloadBlock.getIndex(),
            std::get<std::uint32_t>(_testData["index"]));
}

TEST_F(BlockTest, ShouldReturnCoinbases) {
  const std::vector<std::unique_ptr<Coinbase>>& sCoinbases{
      _coinbaseBlock.getCoinbases().value()};
  ASSERT_EQ(sCoinbases[0]->getOwner(),
            std::get<Coinbase>(_testData["coinbase"]).getOwner());
  ASSERT_EQ(sCoinbases[0]->getBitcoinAmount(),
            std::get<Coinbase>(_testData["coinbase"]).getBitcoinAmount());
  ASSERT_EQ(
      sCoinbases[0]->getBitcoinRepresentation(),
      std::get<Coinbase>(_testData["coinbase"]).getBitcoinRepresentation());
  ASSERT_EQ(sCoinbases[0]->getSatoshiAmount(),
            std::get<Coinbase>(_testData["coinbase"]).getSatoshiAmount());
  ASSERT_EQ(
      sCoinbases[0]->getTimestamp(),
      std::get<transaction::Coinbase>(_testData["coinbase"]).getTimestamp());
}

TEST_F(BlockTest, ShouldReturnPayloads) {
  const std::vector<std::unique_ptr<Payload>>& sPayloads{
      _payloadBlock.getPayloads().value()};
  ASSERT_EQ(sPayloads[0]->getOwner(),
            std::get<Payload>(_testData["payload"]).getOwner());
  ASSERT_EQ(sPayloads[0]->getReceiver(),
            std::get<Payload>(_testData["payload"]).getReceiver());
  ASSERT_EQ(sPayloads[0]->getBitcoinAmount(),
            std::get<Payload>(_testData["payload"]).getBitcoinAmount());
  ASSERT_EQ(sPayloads[0]->getBitcoinRepresentation(),
            std::get<Payload>(_testData["payload"]).getBitcoinRepresentation());
  ASSERT_EQ(sPayloads[0]->getSatoshiAmount(),
            std::get<Payload>(_testData["payload"]).getSatoshiAmount());
  ASSERT_EQ(
      sPayloads[0]->getTimestamp(),
      std::get<transaction::Payload>(_testData["payload"]).getTimestamp());
}

TEST_F(BlockTest, ShouldReturnNullPayloadsWhenBlockContainsOnlyCoinbases) {
  ASSERT_FALSE(_coinbaseBlock.getPayloads().has_value());
}

TEST_F(BlockTest, ShouldReturnNullCoinbasesWhenBlockContainsOnlyPayloads) {
  ASSERT_FALSE(_payloadBlock.getCoinbases().has_value());
}

TEST_F(BlockTest, ShouldReturnNotNullCoinbasesAndPayloadsWhenBlockHasAllData) {
  ASSERT_TRUE(_fullBlock.getCoinbases().has_value());
  ASSERT_TRUE(_fullBlock.getPayloads().has_value());
}
} // namespace tests
} // namespace block