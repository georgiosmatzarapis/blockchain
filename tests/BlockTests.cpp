// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "Block.hpp"
#include "Common.hpp"

namespace block {
namespace tests {

using namespace utils;

class BlockTest : public ::testing::Test {
 protected:
  BlockTest()
      : _testData{{"previousHash", std::string{"dummyHash"}},
                  {"index", static_cast<std::uint32_t>(1)},
                  {"payload", Payload{"Owner", "Receiver", 1.2}},
                  {"coinbase", Coinbase{"Owner", 1.2}}} {
    _coinbases.push_back(
        std::make_unique<Coinbase>(std::get<Coinbase>(_testData["coinbase"])));
    _payloads.push_back(
        std::make_unique<Payload>(std::get<Payload>(_testData["payload"])));
    _fullBlockCoinbases.push_back(
        std::make_unique<Coinbase>(std::get<Coinbase>(_testData["coinbase"])));
    _fullBlockPayloads.push_back(
        std::make_unique<Payload>(std::get<Payload>(_testData["payload"])));
    _coinbaseBlock = Block{std::get<std::string>(_testData["previousHash"]),
                           std::get<std::uint32_t>(_testData["index"]),
                           std::move(_coinbases)};
    _payloadBlock = Block{std::get<std::string>(_testData["previousHash"]),
                          std::get<std::uint32_t>(_testData["index"]),
                          std::move(_payloads)};
    _fullBlock =
        Block{std::get<std::string>(_testData["previousHash"]),
              std::get<std::uint32_t>(_testData["index"]),
              std::move(_fullBlockCoinbases), std::move(_fullBlockPayloads)};
  }

  std::map<std::string, std::variant<std::string, std::uint32_t, std::uint64_t,
                                     Payload, Coinbase>>
      _testData{};
  std::vector<std::unique_ptr<Coinbase>> _coinbases{};
  std::vector<std::unique_ptr<Payload>> _payloads{};
  std::vector<std::unique_ptr<Coinbase>> _fullBlockCoinbases{};
  std::vector<std::unique_ptr<Payload>> _fullBlockPayloads{};
  Block _payloadBlock{};
  Block _coinbaseBlock{};
  Block _fullBlock{};
};

TEST(BlockInitializationTest, ShouldStoreAllValidTransactions) {
  /* Prepare Block instance */
  std::vector<std::unique_ptr<Coinbase>> sCoinbases{};
  std::vector<std::unique_ptr<Payload>> sPayloads{};
  sCoinbases.push_back(
      std::make_unique<Coinbase>(std::string{"dummyOwnerOne"}, 1));
  sCoinbases.push_back(
      std::make_unique<Coinbase>(std::string{"dummyOwnerTwo"}, 2));
  sCoinbases.push_back(
      std::make_unique<Coinbase>(std::string{"dummyOwnerThree"}, 3));
  sPayloads.push_back(std::make_unique<Payload>(
      std::string{"dummyOwnerOne"}, std::string{"dummyReceiverOne"}, 1));
  sPayloads.push_back(std::make_unique<Payload>(
      std::string{"dummyOwnerTwo"}, std::string{"dummyReceiverTwo"}, 2));
  sPayloads.push_back(std::make_unique<Payload>(
      std::string{"dummyOwnerThree"}, std::string{"dummyReceiverThree"}, 3));
  const Block sBlock{std::string{"dummyPreviousHash"}, 0, std::move(sCoinbases),
                     std::move(sPayloads)};

  /* Retrieve data from sBlock */
  const std::vector<std::unique_ptr<Coinbase>>& sBlockCoinbases{
      sBlock.getCoinbases().value()};
  EXPECT_TRUE(sBlockCoinbases.size() == 3);
  const std::vector<std::unique_ptr<Payload>>& sBlockPayloads{
      sBlock.getPayloads().value()};
  EXPECT_TRUE(sBlockPayloads.size() == 3);

  /* Validate Coinbases */
  ASSERT_EQ(sBlockCoinbases[0]->getOwner(), std::string{"dummyOwnerOne"});
  ASSERT_EQ(sBlockCoinbases[0]->getBitcoinAmount(), 1);
  ASSERT_EQ(sBlockCoinbases[1]->getOwner(), std::string{"dummyOwnerTwo"});
  ASSERT_EQ(sBlockCoinbases[1]->getBitcoinAmount(), 2);
  ASSERT_EQ(sBlockCoinbases[2]->getOwner(), std::string{"dummyOwnerThree"});
  ASSERT_EQ(sBlockCoinbases[2]->getBitcoinAmount(), 3);

  /* Validate Payloads */
  ASSERT_EQ(sBlockPayloads[0]->getOwner(), std::string{"dummyOwnerOne"});
  ASSERT_EQ(sBlockPayloads[0]->getReceiver(), std::string{"dummyReceiverOne"});
  ASSERT_EQ(sBlockPayloads[0]->getBitcoinAmount(), 1);
  ASSERT_EQ(sBlockPayloads[1]->getOwner(), std::string{"dummyOwnerTwo"});
  ASSERT_EQ(sBlockPayloads[1]->getReceiver(), std::string{"dummyReceiverTwo"});
  ASSERT_EQ(sBlockPayloads[1]->getBitcoinAmount(), 2);
  ASSERT_EQ(sBlockPayloads[2]->getOwner(), std::string{"dummyOwnerThree"});
  ASSERT_EQ(sBlockPayloads[2]->getReceiver(),
            std::string{"dummyReceiverThree"});
  ASSERT_EQ(sBlockPayloads[2]->getBitcoinAmount(), 3);
}

TEST(BlockInitializationTest, ShouldThrowWhenNoValidTransactionHashFound) {
  std::vector<std::unique_ptr<Coinbase>> sCoinbases{};
  sCoinbases.push_back(std::make_unique<Coinbase>("owner", 1));
  auto sMovedCoinbases{std::move(sCoinbases)};
  ASSERT_THROW(Block("dummyHash", 1, std::move(sCoinbases)),
               core_lib::exception::TransactionConsistencyError);
}

TEST_F(BlockTest, ShouldReturnMerkleRootHashWhenOnlyOneTransactionHashExists) {
  const std::string sExpectedMerkleRootHash{
      core_lib::ComputeHash(_payloadBlock.getPayloads().value()[0]->getHash())
          .value()};
  ASSERT_EQ(_payloadBlock.getMerkleRootHash(), sExpectedMerkleRootHash);
}

TEST_F(BlockTest, ShouldReturnMerkleRootHashWhenTwoTransactionHashesExist) {
  const std::string sExpectedMerkleRootHash{
      core_lib::ComputeHash(_fullBlock.getCoinbases().value()[0]->getHash() +
                            _fullBlock.getPayloads().value()[0]->getHash())
          .value()};
  ASSERT_EQ(_fullBlock.getMerkleRootHash(), sExpectedMerkleRootHash);
}

TEST(BlockMerkleRootHashTest,
     ShouldReturnMerkleRootHashWhenThreeTransactionHashesExist) {
  std::vector<std::unique_ptr<Coinbase>> sCoinbases{};
  sCoinbases.push_back(std::make_unique<Coinbase>("owner", 1));
  sCoinbases.push_back(std::make_unique<Coinbase>("owner", 1));
  std::vector<std::unique_ptr<Payload>> sPayloads{};
  sPayloads.push_back(std::make_unique<Payload>("owner", "receiver", 1));
  Block sBlock{"dummyHash", 1, std::move(sCoinbases), std::move(sPayloads)};

  // Merkle root hash calculation
  const std::string sHashedTwoFirstTransactionHashes{
      core_lib::ComputeHash(sBlock.getCoinbases().value()[0]->getHash() +
                            sBlock.getCoinbases().value()[1]->getHash())
          .value()};
  const std::string sHashedThirdTransactionHash{
      core_lib::ComputeHash(sBlock.getPayloads().value()[0]->getHash())
          .value()};
  const std::string sExpectedMerkleRootHash{
      core_lib::ComputeHash(sHashedTwoFirstTransactionHashes +
                            sHashedThirdTransactionHash)
          .value()};
  ASSERT_EQ(sBlock.getMerkleRootHash(), sExpectedMerkleRootHash);
}

TEST_F(BlockTest, ShouldReturnExpectedHash) {
  const std::string sBlockHeader{std::to_string(_fullBlock.getIndex()) +
                                 _fullBlock.getPreviousHash() +
                                 _fullBlock.getMerkleRootHash() +
                                 std::to_string(_fullBlock.getCreationTime()) +
                                 std::to_string(_fullBlock.getNonce())};
  ASSERT_TRUE(
      core_lib::IsHashValid(sBlockHeader, _fullBlock.getHash()).value());
}

TEST_F(BlockTest, ShouldReturnPreviousHash) {
  ASSERT_EQ(_payloadBlock.getPreviousHash(),
            std::get<std::string>(_testData["previousHash"]));
}

TEST_F(BlockTest, ShouldReturnIndex) {
  ASSERT_EQ(_payloadBlock.getIndex(),
            std::get<std::uint32_t>(_testData["index"]));
}

TEST_F(BlockTest, ShouldReturnNoEmptyNonce) {
  EXPECT_GE(_payloadBlock.getNonce(), 0);
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
  ASSERT_EQ(sCoinbases[0]->getHash(),
            std::get<Coinbase>(_testData["coinbase"]).getHash());
  ASSERT_EQ(sCoinbases[0]->getUnixTimestamp(),
            std::get<Coinbase>(_testData["coinbase"]).getUnixTimestamp());
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
  ASSERT_EQ(sPayloads[0]->getHash(),
            std::get<Payload>(_testData["payload"]).getHash());
  ASSERT_EQ(sPayloads[0]->getUnixTimestamp(),
            std::get<Payload>(_testData["payload"]).getUnixTimestamp());
}

TEST_F(BlockTest, ShouldReturnNullPayloadsWhenBlockContainsOnlyCoinbases) {
  ASSERT_FALSE(_coinbaseBlock.getPayloads().has_value());
  ASSERT_TRUE(_coinbaseBlock.getCoinbases().has_value());
}

TEST_F(BlockTest, ShouldReturnNullCoinbasesWhenBlockContainsOnlyPayloads) {
  ASSERT_FALSE(_payloadBlock.getCoinbases().has_value());
  ASSERT_TRUE(_payloadBlock.getPayloads().has_value());
}

TEST_F(BlockTest, ShouldReturnNotNullCoinbasesAndPayloadsWhenBlockHasAllData) {
  ASSERT_TRUE(_fullBlock.getCoinbases().has_value());
  ASSERT_TRUE(_fullBlock.getPayloads().has_value());
}
} // namespace tests
} // namespace block