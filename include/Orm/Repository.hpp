#pragma once

#include "Orm/DbConnection.hpp"
#include "Orm/Defs.hpp"
#include "Orm/EntityIf.hpp"
#include "Orm/RawResultIf.hpp"
#include "Orm/TableSchema.hpp"
#include <algorithm>
#include <concepts>
#include <iomanip>
#include <map>
#include <memory>
#include <openssl/sha.h>
#include <optional>
#include <sstream>
#include <string>
#include <vector>

namespace Zef::Orm {

template<typename T>
  requires std::derived_from<T, EntityIf>
class Repository {
public:
  static std::optional<std::vector<T>> GetAll() {
    return GetAll(std::string{}, 0);
  }

  static std::optional<std::vector<T>> GetAllRaw(const std::string &query) {
    T proto;
    const auto *schema = proto.GetSchema();
    if (!schema) return std::nullopt;
    return GetAll(query, schema);
  }

  static std::optional<std::vector<T>> GetAllRaw(const std::string &query, const std::vector<std::string> &params) {
    T proto;
    const auto *schema = proto.GetSchema();
    if (!schema) return std::nullopt;
    return GetAll(query, schema, params);
  }

  static std::optional<std::vector<T>> GetAll(const std::string &sortBy) {
    return GetAll(sortBy, 0);
  }
  static std::optional<std::vector<T>> GetAll(const std::map<std::string, std::string> &filters) {
    return GetAll(filters, {}, 0);
  }
  static std::optional<std::vector<T>> GetAll(const std::string &sortBy, int limit) {
    T proto;
    const auto *schema = proto.GetSchema();
    if (!schema) return std::nullopt;

    const std::string query = BuildQuery(schema, {}, sortBy, limit);

    return GetAll(query, schema);
  }
  static std::optional<std::vector<T>> GetAll(const std::map<std::string, std::string> &filters, int limit) {
    return GetAll(filters, {}, limit);
  }
  static std::optional<std::vector<T>> GetAll(const std::map<std::string, std::string> &filters, const std::string &sortBy, int limit) {
    T proto;
    const auto *schema = proto.GetSchema();
    if (!schema) return std::nullopt;

    const std::string query = BuildQuery(schema, filters, sortBy, limit);

    std::vector<std::string> params;
    params.reserve(filters.size());
    for (const auto &[_, val] : filters) params.push_back(val);

    return GetAll(query, schema, params);
  }

private:
  static std::string BuildQuery(const TableSchema *schema, const std::map<std::string, std::string> &filters, const std::string &sortBy, int limit) {
    std::string query = "SELECT ";
    bool first = true;
    for (const auto &col : schema->GetColumns()) {
      if (col->GetFlags() & ColumnFlags::IgnoreOnRead) continue;
      if (!first) query += ", ";
      query += col->GetName();
      first = false;
    }
    query += " FROM " + schema->GetTableName();

    if (!filters.empty()) {
      query += " WHERE ";
      int idx = 1;
      bool first2 = true;
      for (const auto &[col, _] : filters) {
        if (!first2) query += " AND ";
        query += col + " = $" + std::to_string(idx++);
        first2 = false;
      }
    }

    query += " ORDER BY " + (sortBy.empty() ? "id ASC" : sortBy);
    if (limit > 0) query += " LIMIT " + std::to_string(limit);

    return query;
  }

  static std::optional<std::vector<T>> GetAll(const std::string &query, const TableSchema *schema) {
    if (!schema) return std::nullopt;

    auto conn = DbConnection::CreateConnection();
    if (!conn) return std::nullopt;

    auto rawResult = conn->GetAll(query);
    if (!rawResult) return std::nullopt;

    return MapResult(*rawResult, schema);
  }

  static std::optional<std::vector<T>> GetAll(const std::string &query, const TableSchema *schema, const std::vector<std::string> &params) {
    if (!schema) return std::nullopt;

    unsigned char sha1[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(query.c_str()), query.size(), sha1);
    std::ostringstream stmtName;
    stmtName << std::hex << std::setfill('0');
    for (unsigned char byte : sha1) stmtName << std::setw(2) << static_cast<int>(byte);

    auto conn = DbConnection::CreateConnection();
    if (!conn) return std::nullopt;

    auto rawResult = conn->GetAll(query, stmtName.str(), params);
    if (!rawResult) return std::nullopt;

    return MapResult(*rawResult, schema);
  }

  static std::vector<T> MapResult(const std::unique_ptr<RawResultIf> &rawResult, const TableSchema *schema) {
    const auto &columnNames = rawResult->GetColumnNames();
    const auto &data = rawResult->GetData();
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
