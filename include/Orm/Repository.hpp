#pragma once

#include "Orm/DbConnectionIf.hpp"
#include "Orm/Defs.hpp"
#include "Orm/EntityIf.hpp"
#include <algorithm>
#include <concepts>
#include <memory>
#include <optional>
#include <string>
#include <vector>

namespace Zef::Orm {

template<typename T>
  requires std::derived_from<T, EntityIf>
class Repository {
public:
  static std::optional<std::vector<T>> GetAll(std::shared_ptr<DbConnectionIf> conn) {
    T proto;
    const auto *schema = proto.GetSchema();
    if (!schema) return std::nullopt;

    std::string query = "SELECT ";
    bool first = true;
    for (const auto &col : schema->GetColumns()) {
      if (col->GetFlags() & ColumnFlags::IgnoreOnRead) continue;
      if (!first) query += ", ";
      query += col->GetName();
      first = false;
    }
    query += " FROM " + schema->GetTableName();

    auto rawResult = conn->GetAll(query);
    if (!rawResult) return std::nullopt;

    const auto &columnNames = (*rawResult)->GetColumnNames();
    const auto &data = (*rawResult)->GetData();
    const auto &cols = schema->GetColumns();

    std::vector<T> result;
    result.reserve(data.size());
    for (const auto &row : data) {
      T entity;
      for (size_t i = 0; i < columnNames.size() && i < row.size(); ++i) {
        if (row[i].empty()) continue;
        auto it = std::find_if(cols.begin(), cols.end(), [&](const auto &c) { return c->GetName() == columnNames[i]; });
        if (it == cols.end()) continue;
        switch ((*it)->GetType()) {
          case ColumnType::Integer: entity.Set(columnNames[i], std::stoi(row[i])); break;
          case ColumnType::Boolean: entity.Set(columnNames[i], row[i] == "t" || row[i] == "true" || row[i] == "1"); break;
          case ColumnType::Float:   entity.Set(columnNames[i], std::stof(row[i])); break;
          case ColumnType::Text:    entity.Set(columnNames[i], row[i]); break;
        }
      }
      result.push_back(std::move(entity));
    }
    return result;
  }
};

} // namespace Zef::Orm
