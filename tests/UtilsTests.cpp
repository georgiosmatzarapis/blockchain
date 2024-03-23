// author: georgiosmatzarapis

#include <cstring>
#include <gtest/gtest.h>

#include "Hmac.hpp"
#include "OpenSslApi.hpp"
#include "OpenSslApiMock.hpp"

namespace utils {
namespace tests {

using ::testing::_;
using ::testing::Return;

class HmacItTest : public ::testing::Test {
 protected:
  const unsigned char* _message{};
  std::size_t _messageSize{};
  unsigned char* _digest{};
  unsigned int _digestSize{};
  std::unique_ptr<openssl::Api> _openSslApi{};
  std::unique_ptr<mocks::OpenSslApi> _mockOpenSslApi{};

  void SetUp() override {
    _message = reinterpret_cast<const unsigned char*>("dummyText");
    _messageSize = std::strlen(reinterpret_cast<const char*>(_message));
    _openSslApi = std::make_unique<openssl::Api>();
    _mockOpenSslApi = std::make_unique<mocks::OpenSslApi>();
  }

  void TearDown() override { _digest = nullptr; }
};

TEST_F(HmacItTest, ShouldComputeSameHashWhenMessageIsTheSame) {
  unsigned char* sDigestFirstAttempt{};
  unsigned int sDigestSizeFirstAttempt{};
  HmacIt(_message, _messageSize, &sDigestFirstAttempt, &sDigestSizeFirstAttempt,
         std::make_unique<openssl::Api>());
  const std::string sDigestStrFirstAttempt(
      reinterpret_cast<char*>(sDigestFirstAttempt), sDigestSizeFirstAttempt);
  EXPECT_TRUE(sDigestFirstAttempt);

  unsigned char* sDigestSecondAttempt{};
  unsigned int sDigestSizeSecondAttempt{};
  HmacIt(_message, _messageSize, &sDigestSecondAttempt,
         &sDigestSizeSecondAttempt, std::make_unique<openssl::Api>());
  const std::string sDigestStrSecondAttempt(
      reinterpret_cast<char*>(sDigestSecondAttempt), sDigestSizeSecondAttempt);
  EXPECT_TRUE(sDigestSecondAttempt);

  ASSERT_EQ(sDigestStrFirstAttempt, sDigestStrSecondAttempt);

  sDigestFirstAttempt = nullptr;
  sDigestSecondAttempt = nullptr;
}

TEST_F(HmacItTest, ShouldReturnWhenMessageIsNull) {
  HmacIt(nullptr, _messageSize, &_digest, &_digestSize, std::move(_openSslApi));
  ASSERT_FALSE(_digest);
}

TEST_F(HmacItTest, ShouldReturnWhenMessageSizeIsNull) {
  HmacIt(_message, std::size_t{}, &_digest, &_digestSize,
         std::move(_openSslApi));
  ASSERT_FALSE(_digest);
}

TEST_F(HmacItTest, ShouldReturnWhenDigestSizeIsNull) {
  HmacIt(_message, _messageSize, &_digest, nullptr, std::move(_openSslApi));
  ASSERT_FALSE(_digest);
}

TEST_F(HmacItTest, ShouldDigestBeNullWhenNewContextReturnNull) {
  EXPECT_CALL(*_mockOpenSslApi, newContext()).WillOnce(Return(nullptr));
  HmacIt(_message, _messageSize, &_digest, &_digestSize,
         std::move(_mockOpenSslApi));
  ASSERT_FALSE(_digest);
}

TEST_F(HmacItTest, ShouldDigestBeNullWhenDigestInitDoesNotReturnOne) {
  EXPECT_CALL(*_mockOpenSslApi, newContext())
      .WillOnce(Return(EVP_MD_CTX_new()));
  EXPECT_CALL(*_mockOpenSslApi, digestInit(_, _, _)).WillOnce(Return(0));
  HmacIt(_message, _messageSize, &_digest, &_digestSize,
         std::move(_mockOpenSslApi));
  ASSERT_FALSE(_digest);
}

TEST_F(HmacItTest, ShouldDigestBeNullWhenDigestUpdateDoesNotReturnOne) {
  EXPECT_CALL(*_mockOpenSslApi, newContext())
      .WillOnce(Return(EVP_MD_CTX_new()));
  EXPECT_CALL(*_mockOpenSslApi, digestInit(_, _, _)).WillOnce(Return(1));
  EXPECT_CALL(*_mockOpenSslApi, digestUpdate(_, _, _)).WillOnce(Return(0));
  HmacIt(_message, _messageSize, &_digest, &_digestSize,
         std::move(_mockOpenSslApi));
  ASSERT_FALSE(_digest);
}

TEST_F(HmacItTest, ShouldDigestBeNullWhenGetMdSizeReturnZero) {
  EXPECT_CALL(*_mockOpenSslApi, newContext())
      .WillOnce(Return(EVP_MD_CTX_new()));
  EXPECT_CALL(*_mockOpenSslApi, digestInit(_, _, _)).WillOnce(Return(1));
  EXPECT_CALL(*_mockOpenSslApi, digestUpdate(_, _, _)).WillOnce(Return(1));
  EXPECT_CALL(*_mockOpenSslApi, getMdSize(_)).WillOnce(Return(0));
  HmacIt(_message, _messageSize, &_digest, &_digestSize,
         std::move(_mockOpenSslApi));
  ASSERT_FALSE(_digest);
}

TEST_F(HmacItTest, ShouldDigestBeNullWhenDigestFinalDoesNotReturnOne) {
  EXPECT_CALL(*_mockOpenSslApi, newContext())
      .WillOnce(Return(EVP_MD_CTX_new()));
  EXPECT_CALL(*_mockOpenSslApi, digestInit(_, _, _)).WillOnce(Return(1));
  EXPECT_CALL(*_mockOpenSslApi, digestUpdate(_, _, _)).WillOnce(Return(1));
  EXPECT_CALL(*_mockOpenSslApi, getMdSize(_)).WillOnce(Return(1));
  EXPECT_CALL(*_mockOpenSslApi, digestFinal(_, _, _)).WillOnce(Return(0));
  HmacIt(_message, _messageSize, &_digest, &_digestSize,
         std::move(_mockOpenSslApi));
  ASSERT_FALSE(_digest);
}

} // namespace tests
} // namespace utils