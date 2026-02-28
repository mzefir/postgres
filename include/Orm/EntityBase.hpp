#pragma once

#include "Orm/EntityIf.hpp"

namespace Zef::Orm {

class EntityBase : public EntityIf {
public:
  ~EntityBase() override = default;

  std::optional<int> GetInt(const std::string &columnName) const override;
  std::optional<bool> GetBool(const std::string &columnName) const override;
  std::optional<float> GetFloat(const std::string &columnName) const override;
  std::optional<std::string> GetString(const std::string &columnName) const override;

  using EntityIf::Set;
  void Set(const std::string &columnName, int value) override;
  void Set(const std::string &columnName, bool value) override;
  void Set(const std::string &columnName, float value) override;
  void Set(const std::string &columnName, std::string value) override;
};

} // namespace Zef::Orm
