#pragma once

#include "Orm/Connection.hpp"
#include <memory>
#include <string>

namespace Zef::Orm {

class Database {
public:
  Database() = delete;
  Database(const Database &) = delete;
  Database &operator=(const Database &) = delete;
  Database(std::string host, std::string port, std::string dbName, std::string user, std::string password);
  ~Database() = default;

  std::unique_ptr<Connection> CreateConnection();

private:
  std::string m_host;
  std::string m_port;
  std::string m_dbName;
  std::string m_user;
  std::string m_password;
};

} // namespace Zef::Orm
