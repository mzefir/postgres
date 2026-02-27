#pragma once

#include "Orm/RawResultIf.hpp"
#include <string>
#include <vector>

namespace Zef::Orm {

class RawResult : public RawResultIf {
public:
  RawResult() = default;
  ~RawResult() override = default;

  void AddColumn(const std::string &columnName) override;
  void AddRow(std::vector<std::string> row) override;

  const std::vector<std::string> &GetColumnNames() const override;
  const std::vector<std::vector<std::string>> &GetData() const override;

private:
  std::vector<std::string> m_columnNames;
  std::vector<std::vector<std::string>> m_data;
};

} // namespace Zef::Orm
