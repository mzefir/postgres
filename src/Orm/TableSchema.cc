#include "Orm/TableSchema.hpp"

namespace Zef::Orm {

TableSchema::TableSchema(std::string tableName, std::vector<std::unique_ptr<ColumnIf>> columns)
  : m_tableName(std::move(tableName)), m_columns(std::move(columns)) {}

TableSchema::~TableSchema() = default;

const std::string &TableSchema::GetTableName() const { return m_tableName; }

const std::vector<std::unique_ptr<ColumnIf>> &TableSchema::GetColumns() const { return m_columns; }

} // namespace Zef::Orm
