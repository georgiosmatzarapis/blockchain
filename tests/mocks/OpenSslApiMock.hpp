// author: georgiosmatzarapis

#pragma once

#include <gmock/gmock.h>

#include "IOpenSslApi.hpp"

namespace utils {
namespace mocks {
class OpenSslApi final : public openssl::IApi {
 public:
  MOCK_METHOD(EVP_MD_CTX*, newContext, (), (override));
  MOCK_METHOD(int, getMdSize, (const EVP_MD* md), (override));
  MOCK_METHOD(int, digestInit,
              (EVP_MD_CTX * ctx, const EVP_MD* type, ENGINE* impl), (override));
  MOCK_METHOD(int, digestUpdate, (EVP_MD_CTX * ctx, const void* d, size_t cnt),
              (override));
  MOCK_METHOD(int, digestFinal,
              (EVP_MD_CTX * ctx, unsigned char* md, unsigned int* s),
              (override));

  ~OpenSslApi() override = default;
};
} // namespace mocks
} // namespace utils