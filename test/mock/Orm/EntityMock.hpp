#pragma once

#include <testing.hpp>

#include "Orm/EntityIf.hpp"

namespace Zef::Testing::Orm {

class EntityMock : public Zef::Orm::EntityIf {
public:
  MOCK_METHOD((std::optional<std::variant<int, bool, float, std::string>>), Get, (const std::string &columnName), (const, override));
  MOCK_METHOD(std::optional<int>, GetInt, (const std::string &columnName), (const, override));
  MOCK_METHOD(std::optional<bool>, GetBool, (const std::string &columnName), (const, override));
  MOCK_METHOD(std::optional<float>, GetFloat, (const std::string &columnName), (const, override));
  MOCK_METHOD(std::optional<std::string>, GetString, (const std::string &columnName), (const, override));
  MOCK_METHOD(const std::string &, GetTableName, (), (const, override));
  MOCK_METHOD(std::optional<int>, Id, (), (const, override));
  MOCK_METHOD((void), Set, (const std::string &columnName, std::variant<int, bool, float, std::string> value), (override));
  MOCK_METHOD(void, Set, (const std::string &columnName, int value), (override));
  MOCK_METHOD(void, Set, (const std::string &columnName, bool value), (override));
  MOCK_METHOD(void, Set, (const std::string &columnName, float value), (override));
  MOCK_METHOD(void, Set, (const std::string &columnName, std::string value), (override));
};

} // namespace Zef::Testing::Orm
