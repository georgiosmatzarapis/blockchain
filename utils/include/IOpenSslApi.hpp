// author: georgiosmatzarapis

#pragma once

#include <openssl/evp.h>

namespace utils {
namespace openssl {
class IApi {
 public:
  virtual EVP_MD_CTX* newContext() = 0;
  virtual int getMdSize(const EVP_MD* md) = 0;
  virtual int digestInit(EVP_MD_CTX* ctx, const EVP_MD* type, ENGINE* impl) = 0;
  virtual int digestUpdate(EVP_MD_CTX* ctx, const void* d, size_t cnt) = 0;
  virtual int digestFinal(EVP_MD_CTX* ctx, unsigned char* md,
                          unsigned int* s) = 0;

  virtual ~IApi() = default;
};
} // namespace openssl
} // namespace utils