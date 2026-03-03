#pragma once

#include "Math/Decimal.hpp"
#include "Orm/TableSchema.hpp"
#include <optional>
#include <string>
#include <variant>

namespace Zef::Orm {

class EntityIf {
public:
  using ColumnValue = std::variant<int, bool, float, std::string, Zef::Math::Decimal>;

  virtual ~EntityIf() = default;

  virtual std::optional<ColumnValue> Get(const std::string &columnName) const = 0;
  virtual std::optional<int> GetInt(const std::string &columnName) const = 0;
  virtual std::optional<bool> GetBool(const std::string &columnName) const = 0;
  virtual std::optional<float> GetFloat(const std::string &columnName) const = 0;
  virtual std::optional<std::string> GetString(const std::string &columnName) const = 0;
  virtual std::optional<Zef::Math::Decimal> GetDecimal(const std::string &columnName) const = 0;
  virtual const std::string &GetTableName() const = 0;
  virtual const TableSchema *GetSchema() const = 0;
  virtual std::optional<int> Id() const = 0;

  virtual void Set(const std::string &columnName, ColumnValue value) = 0;
  virtual void Set(const std::string &columnName, int value) = 0;
  virtual void Set(const std::string &columnName, bool value) = 0;
  virtual void Set(const std::string &columnName, float value) = 0;
  virtual void Set(const std::string &columnName, std::string value) = 0;
  virtual void Set(const std::string &columnName, Zef::Math::Decimal value) = 0;
};

} // namespace Zef::Orm
