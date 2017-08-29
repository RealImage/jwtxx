#ifndef JWTXX_UTILS_H
#define JWTXX_UTILS_H

#include "jwtxx/jwt.h"

#include <memory>
#include <tuple>

#include <openssl/evp.h>
#include <openssl/ec.h>

namespace JWTXX
{
namespace Utils
{

struct PasswordCallbackError : public JWTXX::Key::Error
{
    PasswordCallbackError() noexcept : Error("Can't read password-protected private key without password callback function.") {}
};

struct EVPKeyDeleter
{
    void operator()(EVP_PKEY* key) const noexcept { EVP_PKEY_free(key); }
};
typedef std::unique_ptr<EVP_PKEY, EVPKeyDeleter> EVPKeyPtr;

struct EVPMDCTXDeleter
{
    void operator()(EVP_MD_CTX* ctx) const noexcept { EVP_MD_CTX_destroy(ctx); }
};
typedef std::unique_ptr<EVP_MD_CTX, EVPMDCTXDeleter> EVPMDCTXPtr;

EVPKeyPtr readPEMPrivateKey(const std::string& fileName, const Key::PasswordCallback& cb);
EVPKeyPtr readPEMPublicKey(const std::string& fileName);

std::string OPENSSLError() noexcept;

typedef std::tuple<std::string, std::string, std::string> Triple;

Triple split(const std::string& token);

} // Utils namespace
} // JWTXX namespace

#endif // JWTXX_UTILS_H
