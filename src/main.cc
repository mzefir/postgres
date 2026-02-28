#include "Entity/Category.hpp"
#include "Orm/DbConnection.hpp"
#include "Orm/Repository.hpp"
#include <cstdlib>
#include <iostream>

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

  auto result = Zef::Orm::Repository<Zef::Entity::Category>::GetAll();

  if (!result) {
    std::cerr << "Query failed." << std::endl;
    return 1;
  }

  for (const auto &category : *result) {
    std::cout << "id=" << category.Id().value_or(-1)
              << " title=" << category.GetString("title").value_or("") << std::endl;
  }

  return 0;
}