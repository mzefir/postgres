#include "Orm/Database.hpp"
#include <libpq-fe.h>

namespace Zef::Orm {

Database::Database(std::string host, std::string port, std::string dbName, std::string user, std::string password)
  : m_host(std::move(host)),
    m_port(std::move(port)),
    m_dbName(std::move(dbName)),
    m_user(std::move(user)),
    m_password(std::move(password)) {}

std::unique_ptr<Connection> Database::CreateConnection() {
  std::string connStr =
    "host="     + m_host +
    " port="    + m_port +
    " dbname="  + m_dbName +
    " user="    + m_user +
    " password=" + m_password;

  PGconn *pgconn = PQconnectdb(connStr.c_str());
  return std::unique_ptr<Connection>(new Connection(pgconn));
}

} // namespace Zef::Orm
