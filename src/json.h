#ifndef JWTXX_JSON_H
#define JWTXX_JSON_H

#include "jwtxx/jwt.h"

namespace JWTXX
{

std::string toJSON(const Pairs& data) noexcept;
Pairs fromJSON(const std::string& data);

} // JWTXX namespace

#endif // JWTXX_JSON_H
