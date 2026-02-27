#pragma once

#include "Orm/RawResultIf.hpp"
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Zef::Orm {

class ConnectionIf {
public:
  virtual ~ConnectionIf() = default;

  virtual std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query) = 0;
  virtual std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) = 0;
};

} // namespace Zef::Orm
