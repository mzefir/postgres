#pragma once

#include <string>
#include <vector>

namespace Zef::Orm {

class RawResultIf {
public:
  virtual ~RawResultIf() = default;

  virtual void AddColumn(const std::string &columnName) = 0;
  virtual void AddRow(std::vector<std::string> row) = 0;

  virtual const std::vector<std::string> &GetColumnNames() const = 0;
  virtual const std::vector<std::vector<std::string>> &GetData() const = 0;
};

} // namespace Zef::Orm
