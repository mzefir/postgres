#include "Orm/EntityBase.hpp"

namespace Zef::Orm {

std::optional<int> EntityBase::GetInt(const std::string &columnName) const {
  auto val = Get(columnName);
  if (!val || !std::holds_alternative<int>(*val)) return std::nullopt;
  return std::get<int>(*val);
}

std::optional<bool> EntityBase::GetBool(const std::string &columnName) const {
  auto val = Get(columnName);
  if (!val || !std::holds_alternative<bool>(*val)) return std::nullopt;
  return std::get<bool>(*val);
}

std::optional<float> EntityBase::GetFloat(const std::string &columnName) const {
  auto val = Get(columnName);
  if (!val || !std::holds_alternative<float>(*val)) return std::nullopt;
  return std::get<float>(*val);
}

std::optional<std::string> EntityBase::GetString(const std::string &columnName) const {
  auto val = Get(columnName);
  if (!val || !std::holds_alternative<std::string>(*val)) return std::nullopt;
  return std::get<std::string>(*val);
}

void EntityBase::Set(const std::string &columnName, int value) {
  Set(columnName, std::variant<int, bool, float, std::string>{value});
}

void EntityBase::Set(const std::string &columnName, bool value) {
  Set(columnName, std::variant<int, bool, float, std::string>{value});
}

void EntityBase::Set(const std::string &columnName, float value) {
  Set(columnName, std::variant<int, bool, float, std::string>{value});
}

void EntityBase::Set(const std::string &columnName, std::string value) {
  Set(columnName, std::variant<int, bool, float, std::string>{std::move(value)});
}

} // namespace Zef::Orm
