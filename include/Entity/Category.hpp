#pragma once

#include "Orm/EntityBase.hpp"
#include <string>

namespace Zef::Entity {

class Category : public Zef::Orm::EntityBase {
public:
  Category() = default;
  ~Category() override = default;

  const std::string &GetTableName() const override;
  const Zef::Orm::TableSchema *GetSchema() const override;

private:
  static const std::string s_tableName;
};

} // namespace Zef::Entity
