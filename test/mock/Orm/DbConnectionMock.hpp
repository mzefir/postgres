#pragma once

#include <testing.hpp>

#include "Orm/DbConnectionIf.hpp"

namespace Zef::Testing::Orm {

class ConnectionMock : public Zef::Orm::DbConnectionIf {
public:
  MOCK_METHOD(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>, GetAll, (const std::string &query), (override));
  MOCK_METHOD(std::optional<std::unique_ptr<Zef::Orm::RawResultIf>>, GetAll, (const std::string &query, const std::string &stmtName, const std::vector<std::string> &params), (override));
  MOCK_METHOD(bool, Exec, (const std::string &query), (override));
  MOCK_METHOD(bool, Exec, (const std::string &query, const std::string &stmtName, const std::vector<std::string> &params), (override));
};

} // namespace Zef::Testing::Orm
