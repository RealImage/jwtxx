#include "utils.h"

#include "jwtxx/jwt.h" // Key::Error

#include <openssl/pem.h>
#include <openssl/err.h>

#include <cstring> // strerror
#include <cerrno> // errno

namespace Utils = JWTXX::Utils;

namespace
{

std::string sysError()
{
    return strerror(errno);
}

}

Utils::EVPKeyPtr Utils::readPEMPrivateKey(const std::string& fileName)
{
    FilePtr fp(fopen(fileName.c_str(), "rb"));
    if (!fp)
        throw Key::Error("Can't open key file '" + fileName + "'. " + sysError());
    EVPKeyPtr key(PEM_read_PrivateKey(fp.get(), nullptr, nullptr, nullptr));
    if (!key)
        throw Key::Error("Can't read public key '" + fileName + "'. " + OPENSSLError());
    return key;
}

Utils::EVPKeyPtr Utils::readPEMPublicKey(const std::string& fileName)
{
    FilePtr fp(fopen(fileName.c_str(), "rb"));
    if (!fp)
        throw Key::Error("Can't open key file '" + fileName + "'. " + sysError());
    EVPKeyPtr key(PEM_read_PUBKEY(fp.get(), nullptr, nullptr, nullptr));
    if (!key)
        throw Key::Error("Can't read public key '" + fileName + "'. " + OPENSSLError());
    return key;
}

Utils::ECKeyPtr Utils::readECPrivateKey(const std::string& fileName)
{
    auto pem = readPEMPrivateKey(fileName);
    ECKeyPtr key(EVP_PKEY_get1_EC_KEY(pem.get()));
    if (!key)
        throw Key::Error("Private key '" + fileName + "' is not an Elliptic Curve key.");
    return key;
}

Utils::ECKeyPtr Utils::readECPublicKey(const std::string& fileName)
{
    auto pem = readPEMPublicKey(fileName);
    ECKeyPtr key(EVP_PKEY_get1_EC_KEY(pem.get()));
    if (!key)
        throw Key::Error("Public key '" + fileName + "' is not an Elliptic Curve key.");
    return key;
}

std::string Utils::OPENSSLError()
{
    char buf[256];
    ERR_error_string_n(ERR_get_error(), buf, sizeof(buf));
    return buf;
}