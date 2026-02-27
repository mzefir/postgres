#pragma once

#include <libpq-fe.h>

namespace Zef::Orm {

class Database;

class Connection {
public:
  Connection() = delete;
  Connection(const Connection &) = delete;
  Connection &operator=(const Connection &) = delete;
  ~Connection();

protected:
  explicit Connection(PGconn *conn);

private:
  PGconn *m_conn;

  friend class Database;
};

} // namespace Zef::Orm
