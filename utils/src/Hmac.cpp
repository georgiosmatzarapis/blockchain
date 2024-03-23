// author: georgiosmatzarapis

#include <openssl/err.h>
#include <sstream>

#include "Hmac.hpp"
#include "Logger.hpp"

namespace utils {
void HmacIt(const unsigned char* iMessage, const std::size_t iMessageSize,
            unsigned char** ioDigest, unsigned int* ioDigestSize,
            const std::unique_ptr<openssl::IApi> ioOpenSslApi) {
  EVP_MD_CTX* sMdCtx{};

  if (!iMessage || !iMessageSize || !ioDigest || !ioDigestSize) {
    Log::GetInstance().toFile(
        LogLevel::WARNING,
        std::string{"Null argument(s) passed into the function."},
        __PRETTY_FUNCTION__);
    *ioDigest = nullptr;
    return;
  }

  try {
    if ((sMdCtx = ioOpenSslApi->newContext()) == nullptr) {
      throw std::runtime_error("EVP_MD_CTX_new");
    }

    if (1 != ioOpenSslApi->digestInit(sMdCtx, EVP_sha256(), nullptr)) {
      throw std::runtime_error("EVP_DigestInit_ex");
    }

    if (1 != ioOpenSslApi->digestUpdate(sMdCtx, iMessage, iMessageSize)) {
      throw std::runtime_error("EVP_DigestUpdate");
    }

    if ((*ioDigest = static_cast<unsigned char*>(OPENSSL_malloc(
             ioOpenSslApi->getMdSize(EVP_sha256())))) == nullptr) {
      throw std::runtime_error("OPENSSL_malloc");
    }

    if (1 != ioOpenSslApi->digestFinal(sMdCtx, *ioDigest, ioDigestSize)) {
      throw std::runtime_error("EVP_DigestFinal_ex");
    }

    EVP_MD_CTX_free(sMdCtx);

  } catch (const std::runtime_error& iRuntimeError) {
    std::ostringstream sStrStreamError{};
    sStrStreamError << iRuntimeError.what() << " failed, error 0x" << std::hex
                    << ERR_get_error() << ".";
    Log::GetInstance().toFile(LogLevel::ERROR, sStrStreamError.str(),
                              __PRETTY_FUNCTION__);
    EVP_MD_CTX_free(sMdCtx);
    OPENSSL_free(*ioDigest);
    *ioDigest = nullptr;
  }
}
} // namespace utils