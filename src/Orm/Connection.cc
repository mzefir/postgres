#include "Orm/Connection.hpp"
#include <libpq-fe.h>

namespace Zef::Orm {

Connection::Connection(PGconn *conn) : m_conn(conn) {}

Connection::~Connection() {
  if (m_conn) {
    PQfinish(m_conn);
    m_conn = nullptr;
  }
}

} // namespace Zef::Orm
