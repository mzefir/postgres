#include "Orm/EntityBase.hpp"

namespace Zef::Orm {

std::optional<EntityBase::ColumnValue> EntityBase::Get(const std::string &columnName) const {
  auto it = m_data.find(columnName);
  if (it == m_data.end()) return std::nullopt;
  return it->second;
}

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

std::optional<Zef::Math::Decimal> EntityBase::GetDecimal(const std::string &columnName) const {
  auto val = Get(columnName);
  if (!val || !std::holds_alternative<Zef::Math::Decimal>(*val)) return std::nullopt;
  return std::get<Zef::Math::Decimal>(*val);
}

std::optional<long long> EntityBase::GetLongLong(const std::string &columnName) const {
  auto val = Get(columnName);
  if (!val || !std::holds_alternative<long long>(*val)) return std::nullopt;
  return std::get<long long>(*val);
}

void EntityBase::Set(const std::string &columnName, int value) {
  Set(columnName, EntityBase::ColumnValue{value});
}

void EntityBase::Set(const std::string &columnName, bool value) {
  Set(columnName, EntityBase::ColumnValue{value});
}

void EntityBase::Set(const std::string &columnName, float value) {
  Set(columnName, EntityBase::ColumnValue{value});
}

void EntityBase::Set(const std::string &columnName, std::string value) {
  Set(columnName, EntityBase::ColumnValue{std::move(value)});
}

void EntityBase::Set(const std::string &columnName, Zef::Math::Decimal value) {
  Set(columnName, EntityBase::ColumnValue{std::move(value)});
}

void EntityBase::Set(const std::string &columnName, long long value) {
  Set(columnName, EntityBase::ColumnValue{value});
}

void EntityBase::Set(const std::string &columnName, ColumnValue value) {
  m_data[columnName] = std::move(value);
}

std::optional<int> EntityBase::Id() const { return GetInt("id"); }

} // namespace Zef::Orm
