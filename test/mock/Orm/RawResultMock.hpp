#pragma once

#include <testing.hpp>

#include "Orm/RawResultIf.hpp"

namespace Zef::Testing::Orm {

class RawResultMock : public Zef::Orm::RawResultIf {
public:
  MOCK_METHOD(void, AddColumn, (std::string columnName), (override));
  MOCK_METHOD(void, AddRow, (std::vector<std::string> row), (override));
  MOCK_METHOD(const std::vector<std::string> &, GetColumnNames, (), (const, override));
  MOCK_METHOD(const std::vector<std::vector<std::string>> &, GetData, (), (const, override));
};

} // namespace Zef::Testing::Orm
