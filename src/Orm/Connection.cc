#include "Orm/Connection.hpp"
#include "Orm/RawResult.hpp"
#include <libpq-fe.h>
#include <optional>
#include <vector>

namespace Zef::Orm {

Connection::Connection(PGconn *conn) : m_conn(conn) {}

Connection::~Connection() {
  if (m_conn) {
    PQfinish(m_conn);
    m_conn = nullptr;
  }
}

std::optional<std::unique_ptr<RawResultIf>> Connection::GetAll(const std::string &query) {
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

std::optional<std::unique_ptr<RawResultIf>> Connection::GetAll(const std::string &query, const std::string &stmtName, const std::vector<std::string> &params) {
  if (m_preparedStatements.find(stmtName) == m_preparedStatements.end()) {
    PGresult *prep = PQprepare(m_conn, stmtName.c_str(), query.c_str(), 0, nullptr);
    if (PQresultStatus(prep) != PGRES_COMMAND_OK) {
      PQclear(prep);
      return std::nullopt;
    }
    PQclear(prep);
    m_preparedStatements.insert(stmtName);
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

} // namespace Zef::Orm
