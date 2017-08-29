#ifndef JWTXX_NONEKEY_H
#define JWTXX_NONEKEY_H

#include "keyimpl.h"

namespace JWTXX
{
namespace Keys
{

struct None : public Key::Impl
{
    std::string sign(const void* /*data*/, size_t /*size*/) const override { return {}; }
    bool verify(const void* /*data*/, size_t /*size*/, const std::string& /*signature*/) const override { return true; }
};

} // Keys namespace
} // JWTXX namespace

#endif // JWTXX_NONEKEY_H
