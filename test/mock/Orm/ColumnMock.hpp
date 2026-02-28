#pragma once

#include <testing.hpp>

#include "Orm/ColumnIf.hpp"

namespace Zef::Testing::Orm {

class ColumnMock : public Zef::Orm::ColumnIf {
public:
  MOCK_METHOD(const std::string &, GetName, (), (const, override));
  MOCK_METHOD(Zef::Orm::ColumnType, GetType, (), (const, override));
  MOCK_METHOD(Zef::Orm::ColumnFlags, GetFlags, (), (const, override));
};

} // namespace Zef::Testing::Orm
