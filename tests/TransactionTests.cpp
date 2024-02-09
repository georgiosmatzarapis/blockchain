// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "TransactionPayload.hpp"

class CoinbaseTest : public ::testing::Test {
 protected:
  CoinbaseTest()
      : _testData{{"owner", std::string{"Owner"}},
                  {"bitcoinAmount", 1122.200031100000},
                  {"satoshiAmount", static_cast<std::uint64_t>(112220003110)},
                  {"bitcoinRepresentation", std::string{"1122.2000311"}}},
        _coinbase{std::get<std::string>(_testData["owner"]),
                  std::get<double>(_testData["bitcoinAmount"])} {};

  std::map<std::string, std::variant<std::string, double, std::uint64_t>>
      _testData{};
  transaction::Coinbase _coinbase;
};

class PayloadTest : public CoinbaseTest {
 protected:
  using CoinbaseTest::_testData;

  PayloadTest()
      : _payload{std::get<std::string>(_testData["owner"]),
                 std::get<std::string>(_testData["receiver"]),
                 std::get<double>(_testData["bitcoinAmount"])} {
    _testData.insert({"receiver", std::string{"Receiver"}});
  }

  transaction::Payload _payload;
};

/* === Coinbase Tests === */

TEST_F(CoinbaseTest, ShouldReturnCoinbaseOwner) {
  EXPECT_EQ(_coinbase.getOwner(), std::get<std::string>(_testData["owner"]));
}

TEST_F(CoinbaseTest, ShouldReturnCoinbaseBitcoinAmount) {
  EXPECT_DOUBLE_EQ(_coinbase.getBitcoinAmount(),
                   std::get<double>(_testData["bitcoinAmount"]));
}

TEST_F(CoinbaseTest, ShouldReturnCoinbaseSatoshiAmount) {
  EXPECT_EQ(_coinbase.getSatoshiAmount(),
            std::get<std::uint64_t>(_testData["satoshiAmount"]));
}

TEST_F(CoinbaseTest, ShouldReturnCoinbaseBitcoinRepresentation) {
  EXPECT_EQ(_coinbase.getBitcoinRepresentation(),
            std::get<std::string>(_testData["bitcoinRepresentation"]));
}

TEST(Coinbase, ShouldMoveConstructCoinbase) {
  transaction::Coinbase aSourceCoinbase{"Owner", 1.2};
  transaction::Coinbase aMovedCoinbase{std::move(aSourceCoinbase)};

  EXPECT_EQ(aSourceCoinbase.getOwner(), "");
  EXPECT_EQ(aSourceCoinbase.getBitcoinRepresentation(), "");
  EXPECT_EQ(aSourceCoinbase.getBitcoinAmount(), 0);
  EXPECT_EQ(aSourceCoinbase.getSatoshiAmount(), 0);
  EXPECT_EQ(aSourceCoinbase.getTimestamp(),
            std::chrono::system_clock::time_point::min());

  EXPECT_EQ(aMovedCoinbase.getOwner(), "Owner");
  EXPECT_EQ(aMovedCoinbase.getBitcoinRepresentation(), "1.2");
  EXPECT_EQ(aMovedCoinbase.getBitcoinAmount(), 1.2);
  EXPECT_EQ(aMovedCoinbase.getSatoshiAmount(), 120000000);
}

TEST(Coinbase, ShouldMoveAssignCoinbase) {
  transaction::Coinbase aSourceCoinbase{"Owner1", 1.2};
  transaction::Coinbase aMovedCoinbase{"Owner2", 1.3};
  aMovedCoinbase = std::move(aSourceCoinbase);

  EXPECT_EQ(aSourceCoinbase.getOwner(), "");
  EXPECT_EQ(aSourceCoinbase.getBitcoinRepresentation(), "");
  EXPECT_EQ(aSourceCoinbase.getBitcoinAmount(), 0);
  EXPECT_EQ(aSourceCoinbase.getSatoshiAmount(), 0);
  EXPECT_EQ(aSourceCoinbase.getTimestamp(),
            std::chrono::system_clock::time_point::min());

  EXPECT_EQ(aMovedCoinbase.getOwner(), "Owner1");
  EXPECT_EQ(aMovedCoinbase.getBitcoinRepresentation(), "1.2");
  EXPECT_EQ(aMovedCoinbase.getBitcoinAmount(), 1.2);
  EXPECT_EQ(aMovedCoinbase.getSatoshiAmount(), 120000000);
}

/* === Payload Tests === */

TEST_F(PayloadTest, ShouldReturnPayloadOwner) {
  EXPECT_EQ(_payload.getOwner(), std::get<std::string>(_testData["owner"]));
}

TEST_F(PayloadTest, ShouldReturnPayloadReceiver) {
  EXPECT_EQ(_payload.getReceiver(),
            std::get<std::string>(_testData["receiver"]));
}

TEST_F(PayloadTest, ShouldReturnPayloadBitcoinAmount) {
  EXPECT_EQ(_payload.getBitcoinAmount(),
            std::get<double>(_testData["bitcoinAmount"]));
}

TEST_F(PayloadTest, ShouldReturnPayloadSatoshiAmount) {
  EXPECT_EQ(_payload.getSatoshiAmount(),
            std::get<std::uint64_t>(_testData["satoshiAmount"]));
}

TEST_F(PayloadTest, ShouldReturnPayloadBitcoinRepresentation) {
  EXPECT_EQ(_payload.getBitcoinRepresentation(),
            std::get<std::string>(_testData["bitcoinRepresentation"]));
}

TEST(Payload, ShouldMoveConstructPayload) {
  transaction::Payload aSourcePayload{"Owner", "Receiver", 1.2};
  transaction::Payload aMovedPayload{std::move(aSourcePayload)};

  EXPECT_EQ(aSourcePayload.getOwner(), "");
  EXPECT_EQ(aSourcePayload.getReceiver(), "");
  EXPECT_EQ(aSourcePayload.getBitcoinRepresentation(), "");
  EXPECT_EQ(aSourcePayload.getBitcoinAmount(), 0);
  EXPECT_EQ(aSourcePayload.getSatoshiAmount(), 0);
  EXPECT_EQ(aSourcePayload.getTimestamp(),
            std::chrono::system_clock::time_point::min());

  EXPECT_EQ(aMovedPayload.getOwner(), "Owner");
  EXPECT_EQ(aMovedPayload.getReceiver(), "Receiver");
  EXPECT_EQ(aMovedPayload.getBitcoinRepresentation(), "1.2");
  EXPECT_EQ(aMovedPayload.getBitcoinAmount(), 1.2);
  EXPECT_EQ(aMovedPayload.getSatoshiAmount(), 120000000);
}

TEST(Payload, ShouldMoveAssignPayload) {
  transaction::Payload aSourcePayload{"Owner1", "Receiver1", 1.2};
  transaction::Payload aMovedPayload{"Owner2", "Receiver2", 1.3};
  aMovedPayload = std::move(aSourcePayload);

  EXPECT_EQ(aSourcePayload.getOwner(), "");
  EXPECT_EQ(aSourcePayload.getReceiver(), "");
  EXPECT_EQ(aSourcePayload.getBitcoinRepresentation(), "");
  EXPECT_EQ(aSourcePayload.getBitcoinAmount(), 0);
  EXPECT_EQ(aSourcePayload.getSatoshiAmount(), 0);
  EXPECT_EQ(aSourcePayload.getTimestamp(),
            std::chrono::system_clock::time_point::min());

  EXPECT_EQ(aMovedPayload.getOwner(), "Owner1");
  EXPECT_EQ(aMovedPayload.getReceiver(), "Receiver1");
  EXPECT_EQ(aMovedPayload.getBitcoinRepresentation(), "1.2");
  EXPECT_EQ(aMovedPayload.getBitcoinAmount(), 1.2);
  EXPECT_EQ(aMovedPayload.getSatoshiAmount(), 120000000);
}