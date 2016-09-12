#pragma once

#include <string>

#include <openssl/evp.h>
#include <openssl/crypto.h>
#include <openssl/bio.h>

namespace JWTXX
{
namespace Base64URL
{

class Block
{
    public:
        Block(size_t size) : m_buffer(OPENSSL_malloc(size)), m_size(size) {}
        ~Block() { OPENSSL_free(m_buffer); }
        Block(Block&& rhs) : m_buffer(rhs.m_buffer), m_size(rhs.m_size) { rhs.m_buffer = nullptr; rhs.m_size = 0; }
        Block& operator=(Block&& rhs) { m_buffer = rhs.m_buffer; m_size = rhs.m_size; rhs.m_buffer = nullptr; rhs.m_size = 0; return *this; }

        size_t size() const { return m_size; }
        const void* data() const { return m_buffer; }

        template <typename T = void*>
        T data() { return static_cast<T>(m_buffer); }

        Block shrink(size_t size) { Block block(m_buffer, size); m_buffer = nullptr; m_size = 0; return block; }

        Block(const Block&) = delete;
        Block& operator=(const Block&) = delete;

    private:
        void* m_buffer;
        size_t m_size;

        Block(void* buffer, size_t size) : m_buffer(buffer), m_size(size) {}
};

std::string URLEncode(const std::string& data)
{
    if (data.empty())
        return {};

    std::string res;
    for (size_t i = 0; i < data.size() - 1; ++i)
    {
        if (data[i] == '=') continue;
        if (data[i] == '/' && data[i + 1] == '+')
        {
            res += "_-";
            ++i;
            continue;
        }

        res += data[i];
    }
    return res;
}

std::string URLDecode(const std::string& data)
{
    if (data.empty())
        return {};

    std::string res;
    for (size_t i = 0; i < data.size() - 1; ++i)
    {
        if (data[i] == '_' && data[i + 1] == '-')
        {
            res += "/+";
            ++i;
            continue;
        }

        res += data[i];
    }

    if (res.size() % 4 == 2)
        return res + "==";
    else if (res.size() % 4 == 3)
        return res + "=";

    return res;
}

std::string encode(const Block& block)
{
    BIO* bio = BIO_new(BIO_f_base64());
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(bio, BIO_new(BIO_s_mem()));
    BIO_write(bio, block.data(), block.size());

    std::string res;

    if (BIO_flush(bio) == 1)
    {
        char* buf = nullptr;
        size_t size = BIO_get_mem_data(bio, &buf);
        if (size != 0 && buf != nullptr)
            res.assign(buf, size);
    }

    BIO_free_all(bio);

    return URLEncode(res);
}

Block decode(std::string data)
{
    data = URLDecode(data);

    BIO* bio = BIO_new(BIO_f_base64());
    BIO_set_flags(bio, BIO_FLAGS_BASE64_NO_NL);
    BIO_push(bio, BIO_new_mem_buf(const_cast<char*>(data.c_str()), data.size()));
    Block block(data.size());
    int res = BIO_read(bio, block.data(), block.size());
    BIO_free_all(bio);

    return block.shrink(res);
}

}
}