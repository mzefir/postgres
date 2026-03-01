#pragma once

#include "Orm/DbConnectionIf.hpp"
#include "Orm/DbConnectionParams.hpp"
#include <libpq-fe.h>
#include <map>
#include <mutex>
#include <shared_mutex>
#include <unordered_set>

namespace Zef::Orm {

class DbConnection : public DbConnectionIf {
public:
  DbConnection(const DbConnection &) = delete;
  DbConnection &operator=(const DbConnection &) = delete;
  ~DbConnection() override;

  static void SetConnectionParams(std::string host, std::string port, std::string dbName, std::string user, std::string password);
  static std::unique_ptr<DbConnectionIf> CreateConnection();

  std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query) override;
  std::optional<std::unique_ptr<RawResultIf>> GetAll(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) override;

  bool Exec(const std::string &query) override;
  bool Exec(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) override;

private:
  DbConnection();
  void Initialize(PGconn *conn);
  static void PurgeStaleConnections();

  struct ConnectionSlot {
    DbConnectionIf            *owner{nullptr};
    std::unordered_set<std::string> preparedStatements;
  };

  static DbConnectionParams s_connectionParams;
  static std::map<PGconn *, ConnectionSlot> s_connections;
  static std::shared_mutex s_connectionsMutex;

  PGconn *m_conn{nullptr};
};

} // namespace Zef::Orm
