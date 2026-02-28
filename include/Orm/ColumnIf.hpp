#pragma once

#include "Orm/Defs.hpp"
#include <string>

namespace Zef::Orm {

class ColumnIf {
public:
  virtual ~ColumnIf() = default;

  virtual const std::string &GetName() const = 0;
  virtual ColumnType GetType() const = 0;
  virtual ColumnFlags GetFlags() const = 0;
};

} // namespace Zef::Orm
