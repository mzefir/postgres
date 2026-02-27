#pragma once

#include <string>
#include <vector>

namespace Zef::Orm {

class RawResult {
public:
  RawResult() = default;
  ~RawResult() = default;

  void AddColumn(std::string columnName);
  void AddRow(std::vector<std::string> row);

  const std::vector<std::string> &GetColumnNames() const;
  const std::vector<std::vector<std::string>> &GetData() const;

private:
  std::vector<std::string> m_columnNames;
  std::vector<std::vector<std::string>> m_data;
};

} // namespace Zef::Orm
