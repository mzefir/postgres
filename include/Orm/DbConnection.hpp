#pragma once

#include "Orm/DbConnectionIf.hpp"
#include "Orm/DbConnectionParams.hpp"
#include <libpq-fe.h>
#include <unordered_set>

namespace Zef::Orm {

class DbConnection : public DbConnectionIf {
public:
  DbConnection() = delete;
  DbConnection(const DbConnection &) = delete;
  DbConnection &operator=(const DbConnection &) = delete;
  ~DbConnection() override;

  static void SetConnectionParams(std::string host, std::string port, std::string dbName, std::string user, std::string password);
  static std::unique_ptr<DbConnectionIf> CreateConnection();

  std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query) override;
  std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) override;

  bool Exec(const std::string &query) override;
  bool Exec(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) override;

protected:
  explicit DbConnection(PGconn *conn);

private:
  static DbConnectionParams s_connectionParams;

  PGconn *m_conn;
  std::unordered_set<std::string> m_preparedStatements;
};

} // namespace Zef::Orm
