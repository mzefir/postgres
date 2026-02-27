#include <iostream>
#include <string>
#include <cstdlib>
#include <libpq-fe.h>

static const char *getenv_or(const char *name, const char *fallback) {
  const char *val = std::getenv(name);
  return (val && val[0]) ? val : fallback;
}

int main() {
  std::string connStr =
    std::string("host=")     + getenv_or("ZEF_DBHOST", "localhost") +
    " port="                 + getenv_or("ZEF_DBPORT", "5432") +
    " dbname="               + getenv_or("ZEF_DBNAME", "postgres") +
    " user="                 + getenv_or("ZEF_DBUSER", "postgres") +
    " password="             + getenv_or("ZEF_DBPASS", "");

  PGconn *conn = PQconnectdb(connStr.c_str());

  if (PQstatus(conn) != CONNECTION_OK) {
    std::cerr << "Connection failed: " << PQerrorMessage(conn) << std::endl;
    PQfinish(conn);
    return 1;
  }

  std::cout << "Connected to: " << PQdb(conn) << std::endl;

  PGresult *res = PQexec(conn, "SELECT * FROM category");

  if (PQresultStatus(res) != PGRES_TUPLES_OK) {
    std::cerr << "Query failed: " << PQerrorMessage(conn) << std::endl;
    PQclear(res);
    PQfinish(conn);
    return 1;
  }

  const int cols = PQnfields(res);
  const int rows = PQntuples(res);

  // Print column headers
  for (int col = 0; col < cols; ++col) {
    std::cout << PQfname(res, col);
    if (col < cols - 1) std::cout << " | ";
  }
  std::cout << std::endl;

  // Print rows
  for (int row = 0; row < rows; ++row) {
    for (int col = 0; col < cols; ++col) {
      std::cout << PQgetvalue(res, row, col);
      if (col < cols - 1) std::cout << " | ";
    }
    std::cout << std::endl;
  }

  PQclear(res);
  PQfinish(conn);
  return 0;
}