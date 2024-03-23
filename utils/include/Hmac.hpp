// author: georgiosmatzarapis

#pragma once

#include <memory>

#include "IOpenSslApi.hpp"

namespace utils {
/**
 * @brief Compute the sha256 of a message without key involvement.
 * @param iMessage Message to hash.
 * @param iMessageSize Size of the message.
 * @param ioDigest Placeholder for the digest.
 * @param ioDigestSize Placeholder for the digest size.
 * @param ioOpenSslApi Implementation of utils::openssl::IApi.
 */
void HmacIt(const unsigned char* iMessage, const std::size_t iMessageSize,
            unsigned char** ioDigest, unsigned int* ioDigestSize,
            const std::unique_ptr<openssl::IApi> ioOpenSslApi);
} // namespace utils