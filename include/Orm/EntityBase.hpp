#pragma once

#include "Orm/EntityIf.hpp"
#include <string>
#include <unordered_map>

namespace Zef::Orm {

class EntityBase : public EntityIf {
public:
  ~EntityBase() override = default;

  std::optional<ColumnValue> Get(const std::string &columnName) const override;
  std::optional<int> GetInt(const std::string &columnName) const override;
  std::optional<bool> GetBool(const std::string &columnName) const override;
  std::optional<float> GetFloat(const std::string &columnName) const override;
  std::optional<std::string> GetString(const std::string &columnName) const override;
  std::optional<Zef::Math::Decimal> GetDecimal(const std::string &columnName) const override;
  std::optional<int> Id() const override;

  using EntityIf::Set;
  void Set(const std::string &columnName, ColumnValue value) override;
  void Set(const std::string &columnName, int value) override;
  void Set(const std::string &columnName, bool value) override;
  void Set(const std::string &columnName, float value) override;
  void Set(const std::string &columnName, std::string value) override;
  void Set(const std::string &columnName, Zef::Math::Decimal value) override;

protected:
  std::unordered_map<std::string, ColumnValue> m_data;
};

} // namespace Zef::Orm
