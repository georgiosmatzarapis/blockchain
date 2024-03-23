// author: georgiosmatzarapis

#include "OpenSslApi.hpp"

namespace utils {
namespace openssl {
EVP_MD_CTX* Api::newContext() { return EVP_MD_CTX_new(); }

int Api::getMdSize(const EVP_MD* md) { return EVP_MD_size(md); }

int Api::digestInit(EVP_MD_CTX* ctx, const EVP_MD* type, ENGINE* impl) {
  return EVP_DigestInit_ex(ctx, type, impl);
}

int Api::digestUpdate(EVP_MD_CTX* ctx, const void* d, size_t cnt) {
  return EVP_DigestUpdate(ctx, d, cnt);
}

int Api::digestFinal(EVP_MD_CTX* ctx, unsigned char* md, unsigned int* s) {
  return EVP_DigestFinal_ex(ctx, md, s);
}

Api::~Api() = default;
} // namespace openssl
} // namespace utils