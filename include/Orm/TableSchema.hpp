#pragma once

#include "Orm/ColumnIf.hpp"
#include <memory>
#include <string>
#include <vector>

namespace Zef::Orm {

class TableSchema {
public:
  TableSchema() = delete;
  TableSchema(const TableSchema &) = delete;
  TableSchema &operator=(const TableSchema &) = delete;
  TableSchema(TableSchema &&) = delete;
  TableSchema &operator=(TableSchema &&) = delete;

  TableSchema(std::string tableName, std::vector<std::unique_ptr<ColumnIf>> columns);
  ~TableSchema();

  const std::string &GetTableName() const;
  const std::vector<std::unique_ptr<ColumnIf>> &GetColumns() const;

private:
  std::string m_tableName;
  std::vector<std::unique_ptr<ColumnIf>> m_columns;
};

} // namespace Zef::Orm
