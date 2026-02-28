#include "Orm/DbConnection.hpp"
#include <cstdlib>
#include <iostream>
#include <string>
#include <vector>

static const char *getenv_or(const char *name, const char *fallback) {
  const char *val = std::getenv(name);
  return (val && val[0]) ? val : fallback;
}

int main() {
  Zef::Orm::DbConnection::SetConnectionParams(
    getenv_or("ZEF_DBHOST", "localhost"),
    getenv_or("ZEF_DBPORT", "5432"),
    getenv_or("ZEF_DBNAME", "postgres"),
    getenv_or("ZEF_DBUSER", "postgres"),
    getenv_or("ZEF_DBPASS", "")
  );

  auto conn = Zef::Orm::DbConnection::CreateConnection();

  const std::vector<std::string> params = {"30"};
  auto result = conn->GetAll("SELECT * FROM category WHERE id > $1", "get_category", params);

  if (!result) {
    std::cerr << "Query failed." << std::endl;
    return 1;
  }

  const auto &data = result.value();

  // Print column headers
  const auto &cols = data->GetColumnNames();
  for (size_t i = 0; i < cols.size(); ++i) {
    std::cout << cols[i];
    if (i < cols.size() - 1) std::cout << " | ";
  }
  std::cout << std::endl;

  // Print rows
  for (const auto &row : data->GetData()) {
    for (size_t i = 0; i < row.size(); ++i) {
      std::cout << row[i];
      if (i < row.size() - 1) std::cout << " | ";
    }
    std::cout << std::endl;
  }

  return 0;
}