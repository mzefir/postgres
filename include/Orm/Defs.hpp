#pragma once

#include <cstdint>

namespace Zef::Orm {

enum class ColumnType {
  Integer,
  Boolean,
  Float,
  Text,
  Decimal,
  BigInteger
};

enum class ColumnFlags : uint8_t {
  Nullable       = 1 << 0,
  IgnoreOnRead   = 1 << 1,
  IgnoreOnWrite  = 1 << 2,
  OneToMany      = 1 << 3
};

inline ColumnFlags operator|(ColumnFlags a, ColumnFlags b) {
  return static_cast<ColumnFlags>(static_cast<uint8_t>(a) | static_cast<uint8_t>(b));
}

inline bool operator&(ColumnFlags a, ColumnFlags b) {
  return static_cast<uint8_t>(a) & static_cast<uint8_t>(b);
}

} // namespace Zef::Orm
