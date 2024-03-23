// author: georgiosmatzarapis

#pragma once

#include "IOpenSslApi.hpp"

namespace utils {
namespace openssl {
class Api final : public IApi {
 public:
  EVP_MD_CTX* newContext() override;
  int getMdSize(const EVP_MD* md) override;
  int digestInit(EVP_MD_CTX* ctx, const EVP_MD* type, ENGINE* impl) override;
  int digestUpdate(EVP_MD_CTX* ctx, const void* d, size_t cnt) override;
  int digestFinal(EVP_MD_CTX* ctx, unsigned char* md, unsigned int* s) override;

  ~Api() override;
};
} // namespace openssl
} // namespace utils