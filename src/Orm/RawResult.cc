#include "Orm/RawResult.hpp"

namespace Zef::Orm {

void RawResult::AddColumn(std::string columnName) {
  m_columnNames.push_back(std::move(columnName));
}

void RawResult::AddRow(std::vector<std::string> row) {
  m_data.push_back(std::move(row));
}

const std::vector<std::string> &RawResult::GetColumnNames() const {
  return m_columnNames;
}

const std::vector<std::vector<std::string>> &RawResult::GetData() const {
  return m_data;
}

} // namespace Zef::Orm
