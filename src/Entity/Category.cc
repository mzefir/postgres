#include "Entity/Category.hpp"
#include "Orm/Column.hpp"

namespace Zef::Entity {

const std::string Category::s_tableName{"category"};

const std::string &Category::GetTableName() const { return s_tableName; }

const Zef::Orm::TableSchema *Category::GetSchema() const {
  static const std::unique_ptr<Zef::Orm::TableSchema> s_schema = []() {
    std::vector<std::unique_ptr<Zef::Orm::ColumnIf>> cols;
    cols.push_back(std::make_unique<Zef::Orm::Column>("id",   Zef::Orm::ColumnType::Integer, Zef::Orm::ColumnFlags::IgnoreOnWrite));
    cols.push_back(std::make_unique<Zef::Orm::Column>("title", Zef::Orm::ColumnType::Text,    Zef::Orm::ColumnFlags::Nullable));
    return std::make_unique<Zef::Orm::TableSchema>(s_tableName, std::move(cols));
  }();
  return s_schema.get();
}

} // namespace Zef::Entity
