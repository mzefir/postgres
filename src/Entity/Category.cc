#include "Entity/Category.hpp"

namespace Zef::Entity {

const std::string Category::s_tableName{"categories"};

std::optional<Category::ColumnValue> Category::Get(const std::string &columnName) const {
  auto it = m_data.find(columnName);
  if (it == m_data.end()) return std::nullopt;
  return it->second;
}

const std::string &Category::GetTableName() const { return s_tableName; }

const Zef::Orm::TableSchema *Category::GetSchema() const { return nullptr; }

std::optional<int> Category::Id() const { return GetInt("id"); }

void Category::Set(const std::string &columnName, Category::ColumnValue value) {
  m_data[columnName] = std::move(value);
}

} // namespace Zef::Entity
