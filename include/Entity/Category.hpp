#pragma once

#include "Orm/EntityBase.hpp"
#include <string>
#include <unordered_map>
#include <variant>

namespace Zef::Entity {

class Category : public Zef::Orm::EntityBase {
public:
  Category() = default;
  ~Category() override = default;

  std::optional<ColumnValue> Get(const std::string &columnName) const override;
  const std::string &GetTableName() const override;
  const Zef::Orm::TableSchema *GetSchema() const override;
  std::optional<int> Id() const override;

  using EntityBase::Set;
  void Set(const std::string &columnName, ColumnValue value) override;

private:
  static const std::string s_tableName;
  std::unordered_map<std::string, ColumnValue> m_data;
};

} // namespace Zef::Entity
