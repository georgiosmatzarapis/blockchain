// author: georgiosmatzarapis

#include <gtest/gtest.h>

#include "Common.hpp"
#include "Transaction.hpp"

namespace transaction {
namespace tests {

namespace lib {
template <typename Transaction>
static void ASSERT_MOVE_OPERATIONS(
    const Transaction& iSourceTransaction, Transaction& ioMovedTransaction,
    const std::chrono::_V2::system_clock::time_point& iSourceTimestamp,
    const std::time_t& iSourceUnixTimestamp, const std::string& iSourceHash) {
  static_assert(std::is_same<Transaction, Coinbase>::value ||
                    std::is_same<Transaction, Payload>::value,
                "Transaction type must be either Coinbase or Payload");

  ASSERT_EQ(iSourceTransaction.getOwner(), "");
  ASSERT_EQ(iSourceTransaction.getBitcoinRepresentation(), "");
  ASSERT_EQ(iSourceTransaction.getBitcoinAmount(), 0);
  ASSERT_EQ(iSourceTransaction.getSatoshiAmount(), 0);
  ASSERT_EQ(iSourceTransaction.getTimestamp(),
            std::chrono::system_clock::time_point::min());
  ASSERT_EQ(iSourceTransaction.getUnixTimestamp(), std::time_t{946684800});
  ASSERT_EQ(ioMovedTransaction.getOwner(), "Owner");
  ASSERT_EQ(ioMovedTransaction.getBitcoinRepresentation(), "1.2");
  ASSERT_EQ(ioMovedTransaction.getHash(), iSourceHash);
  ASSERT_EQ(ioMovedTransaction.getBitcoinAmount(), 1.2);
  ASSERT_EQ(ioMovedTransaction.getSatoshiAmount(), 120000000);
  ASSERT_EQ(ioMovedTransaction.getTimestamp(), iSourceTimestamp);
  ASSERT_EQ(ioMovedTransaction.getUnixTimestamp(), iSourceUnixTimestamp);

  if constexpr (std::is_same<Transaction, Payload>::value) {
    ASSERT_EQ(iSourceTransaction.getReceiver(), "");
    ASSERT_EQ(ioMovedTransaction.getReceiver(), "Receiver");
  }
}
} // namespace lib

struct Data {
  static constexpr std::string kOwner{"Owner"};
  static constexpr std::string kReceiver{"Receiver"};
  static constexpr double kBitcoinAmount{1122.200031100000};
  static constexpr auto kSatoshiAmount{
      static_cast<std::uint64_t>(112220003110)};
  static constexpr std::string kBitcoinRepresentation{"1122.2000311"};
};

class CoinbaseTest : public ::testing::Test {
 protected:
  CoinbaseTest() : _coinbase{Data::kOwner, Data::kBitcoinAmount} {};

  transaction::Coinbase _coinbase;
};

class PayloadTest : public ::testing::Test {
 protected:
  PayloadTest()
      : _payload{Data::kOwner, Data::kReceiver, Data::kBitcoinAmount} {};

  transaction::Payload _payload;
};

/* === utils::core_lib Tests === */

TEST(ComputeHashTest, ShouldReturnTrueAndHashWhenMessageIsNoEmpty) {
  std::pair<bool, std::optional<std::string>> sHash{
      utils::core_lib::ComputeHash(std::string{"dummyText"})};
  ASSERT_TRUE(sHash.first);
  ASSERT_TRUE(sHash.second.has_value());
}

TEST(ComputeHashTest, ShouldReturnFalseAndNoHashWhenMessageIsEmpty) {
  std::pair<bool, std::optional<std::string>> sHash{
      utils::core_lib::ComputeHash(std::string{""})};
  ASSERT_FALSE(sHash.first);
  ASSERT_FALSE(sHash.second.has_value());
}

TEST(ComputeHashTest, ShouldReturnSameHashWhenMessageIsTheSame) {
  ASSERT_EQ(utils::core_lib::ComputeHash(std::string{"dummyText"}),
            utils::core_lib::ComputeHash(std::string{"dummyText"}));
}

TEST(ComputeHashTest, ShouldReturnDifferentHashWhenMessageIsNotTheSame) {
  ASSERT_NE(utils::core_lib::ComputeHash(std::string{"dummyText1"}),
            utils::core_lib::ComputeHash(std::string{"dummyText2"}));
}

/* === Coinbase Tests === */

TEST_F(CoinbaseTest, ShouldReturnOwner) {
  ASSERT_EQ(_coinbase.getOwner(), Data::kOwner);
}

TEST_F(CoinbaseTest, ShouldReturnBitcoinAmount) {
  ASSERT_DOUBLE_EQ(_coinbase.getBitcoinAmount(), Data::kBitcoinAmount);
}

TEST_F(CoinbaseTest, ShouldReturnSatoshiAmount) {
  ASSERT_EQ(_coinbase.getSatoshiAmount(), Data::kSatoshiAmount);
}

TEST_F(CoinbaseTest, ShouldReturnBitcoinRepresentation) {
  ASSERT_EQ(_coinbase.getBitcoinRepresentation(), Data::kBitcoinRepresentation);
}

TEST_F(CoinbaseTest, ShouldReturnTimestampAndUnixTimestamp) {
  const auto sTimestamp{_coinbase.getTimestamp()};
  EXPECT_NE(sTimestamp, std::chrono::system_clock::time_point{});

  const auto sDurationSinceEpoch{sTimestamp.time_since_epoch()};
  const auto sSeconds{
      std::chrono::duration_cast<std::chrono::seconds>(sDurationSinceEpoch)};
  ASSERT_EQ(_coinbase.getUnixTimestamp(), sSeconds.count());
}

TEST_F(CoinbaseTest, ShouldComputeAndReturnHashWhenHashIsEmpty) {
  ASSERT_TRUE(_coinbase.getHash().size());
}

TEST_F(CoinbaseTest, ShouldReturnTheStoredHashWhenHashExists) {
  const std::string sHashFirstAttempt{_coinbase.getHash()};
  EXPECT_TRUE(sHashFirstAttempt.size());
  const std::string sHashSecondAttempt{_coinbase.getHash()};
  EXPECT_TRUE(sHashSecondAttempt.size());

  ASSERT_EQ(sHashFirstAttempt, sHashSecondAttempt);
}

TEST(Coinbase, ShouldThrowRuntimeErrorWhenHashComputationFails) {
  GTEST_SKIP() << "TODO: Mock 'utils::core_lib::ComputeHash' function to test "
                  "the exception flow in "
                  "'getHash' method.";
}

TEST(Coinbase, ShouldMoveConstruct) {
  transaction::Coinbase sSourceCoinbase{"Owner", 1.2};
  const auto sSourceTimestamp{sSourceCoinbase.getTimestamp()};
  const auto sSourceUnixTimestamp{sSourceCoinbase.getUnixTimestamp()};
  const std::string sSourceHash{sSourceCoinbase.getHash()};
  transaction::Coinbase sMovedCoinbase{std::move(sSourceCoinbase)};
  lib::ASSERT_MOVE_OPERATIONS(sSourceCoinbase, sMovedCoinbase, sSourceTimestamp,
                              sSourceUnixTimestamp, sSourceHash);
}

TEST(Coinbase, ShouldMoveAssign) {
  transaction::Coinbase sSourceCoinbase{"Owner", 1.2};
  const auto sSourceTimestamp{sSourceCoinbase.getTimestamp()};
  const auto sSourceUnixTimestamp{sSourceCoinbase.getUnixTimestamp()};
  const std::string sSourceHash{sSourceCoinbase.getHash()};
  transaction::Coinbase sMovedCoinbase{"Owner1", 1.3};
  sMovedCoinbase = std::move(sSourceCoinbase);
  lib::ASSERT_MOVE_OPERATIONS(sSourceCoinbase, sMovedCoinbase, sSourceTimestamp,
                              sSourceUnixTimestamp, sSourceHash);
}

/* === Payload Tests === */

TEST_F(PayloadTest, ShouldReturnOwner) {
  ASSERT_EQ(_payload.getOwner(), Data::kOwner);
}

TEST_F(PayloadTest, ShouldReturnReceiver) {
  ASSERT_EQ(_payload.getReceiver(), Data::kReceiver);
}

TEST_F(PayloadTest, ShouldReturnBitcoinAmount) {
  ASSERT_EQ(_payload.getBitcoinAmount(), Data::kBitcoinAmount);
}

TEST_F(PayloadTest, ShouldReturnSatoshiAmount) {
  ASSERT_EQ(_payload.getSatoshiAmount(), Data::kSatoshiAmount);
}

TEST_F(PayloadTest, ShouldReturnBitcoinRepresentation) {
  ASSERT_EQ(_payload.getBitcoinRepresentation(), Data::kBitcoinRepresentation);
}

TEST_F(PayloadTest, ShouldReturnTimestampAndUnixTimestamp) {
  const auto sTimestamp{_payload.getTimestamp()};
  EXPECT_NE(sTimestamp, std::chrono::system_clock::time_point{});

  const auto sDurationSinceEpoch{sTimestamp.time_since_epoch()};
  const auto sSeconds{
      std::chrono::duration_cast<std::chrono::seconds>(sDurationSinceEpoch)};
  ASSERT_EQ(_payload.getUnixTimestamp(), sSeconds.count());
}

TEST_F(PayloadTest, ShouldComputeAndReturnHashWhenHashIsEmpty) {
  ASSERT_TRUE(_payload.getHash().size());
}

TEST_F(PayloadTest, ShouldReturnTheStoredHashWhenHashExists) {
  const std::string sHashFirstAttempt{_payload.getHash()};
  EXPECT_TRUE(sHashFirstAttempt.size());
  const std::string sHashSecondAttempt{_payload.getHash()};
  EXPECT_TRUE(sHashSecondAttempt.size());

  ASSERT_EQ(sHashFirstAttempt, sHashSecondAttempt);
}

TEST(Payload, ShouldThrowRuntimeErrorWhenHashComputationFails) {
  GTEST_SKIP() << "TODO: Mock 'utils::core_lib::ComputeHash' function to test "
                  "the exception flow in "
                  "'getHash' method.";
}

TEST(Payload, ShouldMoveConstruct) {
  transaction::Payload sSourcePayload{"Owner", "Receiver", 1.2};
  const auto sSourceTimestamp{sSourcePayload.getTimestamp()};
  const auto sSourceUnixTimestamp{sSourcePayload.getUnixTimestamp()};
  const std::string sSourceHash{sSourcePayload.getHash()};
  transaction::Payload sMovedPayload{std::move(sSourcePayload)};
  lib::ASSERT_MOVE_OPERATIONS(sSourcePayload, sMovedPayload, sSourceTimestamp,
                              sSourceUnixTimestamp, sSourceHash);
}

TEST(Payload, ShouldMoveAssign) {
  transaction::Payload sSourcePayload{"Owner", "Receiver", 1.2};
  const auto sSourceTimestamp{sSourcePayload.getTimestamp()};
  const auto sSourceUnixTimestamp{sSourcePayload.getUnixTimestamp()};
  const std::string sSourceHash{sSourcePayload.getHash()};
  transaction::Payload sMovedPayload{"Owner1", "Receiver1", 1.3};
  sMovedPayload = std::move(sSourcePayload);
  lib::ASSERT_MOVE_OPERATIONS(sSourcePayload, sMovedPayload, sSourceTimestamp,
                              sSourceUnixTimestamp, sSourceHash);
}

/* === Coinbase and Payload Tests === */

TEST(CoinbaseAndPayload, ShouldReturnExpectedAttributeValuesForEachInstance) {
  Coinbase sCoinbase{"CoinbaseOwner", 1.2};
  const std::string sCoinbaseHash{sCoinbase.getHash()};
  Payload sPayload{"PayloadOwner", "PayloadReceiver", 1.3};
  const std::string sPayloadHash{sPayload.getHash()};

  ASSERT_EQ(sCoinbase.getOwner(), "CoinbaseOwner");
  ASSERT_EQ(sCoinbase.getBitcoinRepresentation(), "1.2");
  ASSERT_EQ(sCoinbase.getBitcoinAmount(), 1.2);
  ASSERT_EQ(sCoinbase.getSatoshiAmount(), 120000000);
  ASSERT_EQ(sCoinbase.getHash(), sCoinbaseHash);

  ASSERT_EQ(sPayload.getOwner(), "PayloadOwner");
  ASSERT_EQ(sPayload.getReceiver(), "PayloadReceiver");
  ASSERT_EQ(sPayload.getBitcoinRepresentation(), "1.3");
  ASSERT_EQ(sPayload.getBitcoinAmount(), 1.3);
  ASSERT_EQ(sPayload.getSatoshiAmount(), 130000000);
  ASSERT_EQ(sPayload.getHash(), sPayloadHash);

  ASSERT_NE(sCoinbaseHash, sPayloadHash);
}

} // namespace tests
} // namespace transaction