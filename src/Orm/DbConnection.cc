#include "Orm/DbConnection.hpp"
#include "Orm/RawResult.hpp"
#include <libpq-fe.h>
#include <map>
#include <mutex>
#include <optional>
#include <shared_mutex>
#include <vector>

namespace Zef::Orm {

DbConnectionParams DbConnection::s_connectionParams;
std::map<PGconn *, DbConnection::ConnectionSlot> DbConnection::s_connections;
std::shared_mutex DbConnection::s_connectionsMutex;

void DbConnection::SetConnectionParams(std::string host, std::string port, std::string dbName, std::string user, std::string password) {
  s_connectionParams = {std::move(host), std::move(port), std::move(dbName), std::move(user), std::move(password)};
}

std::unique_ptr<DbConnectionIf> DbConnection::CreateConnection() {
  std::unique_lock lock(s_connectionsMutex);
  PurgeStaleConnections();
  auto db = std::unique_ptr<DbConnection>(new DbConnection());

  PGconn *pgconn = nullptr;
  for (auto &[conn, slot] : s_connections) {
    if (slot.owner == nullptr) {
      pgconn       = conn;
      slot.owner   = db.get();
      break;
    }
  }

  if (!pgconn) {
    std::string connStr =
      "host="      + s_connectionParams.host +
      " port="     + s_connectionParams.port +
      " dbname="   + s_connectionParams.dbName +
      " user="     + s_connectionParams.user +
      " password=" + s_connectionParams.password;

    pgconn = PQconnectdb(connStr.c_str());
    s_connections[pgconn].owner = db.get();
  }

  db->Initialize(pgconn);
  return db;
}

void DbConnection::PurgeStaleConnections() {
  for (auto it = s_connections.begin(); it != s_connections.end(); ) {
    if (it->second.owner == nullptr && PQstatus(it->first) != CONNECTION_OK) {
      PQfinish(it->first);
      it = s_connections.erase(it);
    } else {
      ++it;
    }
  }
}

DbConnection::DbConnection() {}

void DbConnection::Initialize(PGconn *conn) {
  m_conn = conn;
}

DbConnection::~DbConnection() {
  std::unique_lock lock(s_connectionsMutex);
  for (auto &[conn, slot] : s_connections) {
    if (slot.owner == this) {
      slot.owner = nullptr;
      break;
    }
  }
}

std::optional<std::unique_ptr<RawResultIf>> DbConnection::GetAll(const std::string &query) {
  PGresult *res = PQexec(m_conn, query.c_str());

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    PQclear(res);
    return std::nullopt;
  }

  auto result = std::make_unique<RawResult>();

  const int cols = PQnfields(res);
  const int rows = PQntuples(res);

  for (int col = 0; col < cols; ++col)
    result->AddColumn(PQfname(res, col));

  for (int row = 0; row < rows; ++row) {
    std::vector<std::string> rowData;
    for (int col = 0; col < cols; ++col)
      rowData.push_back(PQgetvalue(res, row, col));
    result->AddRow(std::move(rowData));
  }

  PQclear(res);
  return result;
}

std::optional<std::unique_ptr<RawResultIf>> DbConnection::GetAll(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) {
  std::shared_lock lock(s_connectionsMutex);
  auto &preparedStatements = s_connections[m_conn].preparedStatements;
  if (preparedStatements.find(stmtName) == preparedStatements.end()) {
    PGresult *prep = PQprepare(m_conn, stmtName.c_str(), query.c_str(), 0, nullptr);
    if (PQresultStatus(prep) != PGRES_COMMAND_OK) {
      PQclear(prep);
      return std::nullopt;
    }
    PQclear(prep);
    preparedStatements.insert(stmtName);
  }

  std::vector<const char *> paramPtrs;
  for (const auto &p : params) paramPtrs.push_back(p.c_str());

  PGresult *res = PQexecPrepared(m_conn, stmtName.c_str(), static_cast<int>(paramPtrs.size()), paramPtrs.data(), nullptr, nullptr, 0);

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    PQclear(res);
    return std::nullopt;
  }

  auto result = std::make_unique<RawResult>();

  const int cols = PQnfields(res);
  const int rows = PQntuples(res);

  for (int col = 0; col < cols; ++col)
    result->AddColumn(PQfname(res, col));

  for (int row = 0; row < rows; ++row) {
    std::vector<std::string> rowData;
    for (int col = 0; col < cols; ++col)
      rowData.push_back(PQgetvalue(res, row, col));
    result->AddRow(std::move(rowData));
  }

  PQclear(res);
  return result;
}

bool DbConnection::Exec(const std::string &query) {
  PGresult *res = PQexec(m_conn, query.c_str());
  const bool ok = PQresultStatus(res) == PGRES_COMMAND_OK;
  PQclear(res);
  return ok;
}

bool DbConnection::Exec(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) {
  std::shared_lock lock(s_connectionsMutex);
  auto &preparedStatements = s_connections[m_conn].preparedStatements;
  if (preparedStatements.find(stmtName) == preparedStatements.end()) {
    PGresult *prep = PQprepare(m_conn, stmtName.c_str(), query.c_str(), 0, nullptr);
    if (PQresultStatus(prep) != PGRES_COMMAND_OK) {
      PQclear(prep);
      return false;
    }
    PQclear(prep);
    preparedStatements.insert(stmtName);
  }

  std::vector<const char *> paramPtrs;
  for (const auto &p : params) paramPtrs.push_back(p.c_str());

  PGresult *res = PQexecPrepared(m_conn, stmtName.c_str(), static_cast<int>(paramPtrs.size()), paramPtrs.data(), nullptr, nullptr, 0);
  const bool ok = PQresultStatus(res) == PGRES_COMMAND_OK;
  PQclear(res);
  return ok;
}

} // namespace Zef::Orm