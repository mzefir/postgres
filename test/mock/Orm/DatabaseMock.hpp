#pragma once

#include <testing.hpp>

#include "Orm/DatabaseIf.hpp"

namespace Zef::Testing::Orm {

class DatabaseMock : public Zef::Orm::DatabaseIf {
public:
  MOCK_METHOD(std::unique_ptr<Zef::Orm::ConnectionIf>, CreateConnection, (), (override));
};

} // namespace Zef::Testing::Orm
