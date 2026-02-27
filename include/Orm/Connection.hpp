#pragma once

#include "Orm/ConnectionIf.hpp"
#include <libpq-fe.h>
#include <unordered_set>

namespace Zef::Orm {

class Database;

class Connection : public ConnectionIf {
public:
  Connection() = delete;
  Connection(const Connection &) = delete;
  Connection &operator=(const Connection &) = delete;
  ~Connection() override;

  std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query) override;
  std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) override;

protected:
  explicit Connection(PGconn *conn);

private:
  PGconn *m_conn;
  std::unordered_set<std::string> m_preparedStatements;

  friend class Database;
};

} // namespace Zef::Orm
