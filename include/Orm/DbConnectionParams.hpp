#pragma once

#include <string>

namespace Zef::Orm {

struct DbConnectionParams {
  std::string host;
  std::string port;
  std::string dbName;
  std::string user;
  std::string password;
};

} // namespace Zef::Orm
