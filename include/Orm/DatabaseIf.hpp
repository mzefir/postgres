#pragma once

#include "Orm/ConnectionIf.hpp"
#include <memory>

namespace Zef::Orm {

class DatabaseIf {
public:
  virtual ~DatabaseIf() = default;

  virtual std::unique_ptr<ConnectionIf> CreateConnection() = 0;
};

} // namespace Zef::Orm
