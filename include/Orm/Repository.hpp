#pragma once

#include "Orm/DbConnection.hpp"
#include "Orm/Defs.hpp"
#include "Orm/EntityIf.hpp"
#include "Orm/RawResultIf.hpp"
#include "Orm/TableSchema.hpp"
#include <algorithm>
#include <concepts>
#include <functional>
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
  static bool Save(T &entity) {
    const auto id = entity.Id();
    if (!id || *id == 0) return Insert(entity);
    return Update(entity);
  }

  static bool Delete(T &entity) {
    const auto id = entity.Id();
    if (!id || *id == 0) return false;

    const auto *schema = entity.GetSchema();
    if (!schema) return false;

    const std::string query = "DELETE FROM " + schema->GetTableName() + " WHERE id = $1";

    unsigned char sha1[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(query.c_str()), query.size(), sha1);
    std::ostringstream stmtName;
    stmtName << std::hex << std::setfill('0');
    for (unsigned char byte : sha1) stmtName << std::setw(2) << static_cast<int>(byte);

    auto conn = CreateConnection();
    if (!conn) return false;

    if (!conn->Exec(query, stmtName.str(), {std::to_string(*id)})) return false;

    entity.Set("id", 0);
    return true;
  }

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

#ifdef UNITTEST
public:
  static void SetConnectionFactory(std::function<std::unique_ptr<DbConnectionIf>()> factory) { s_connectionFactory = std::move(factory); }
#endif

private:
  static inline std::function<std::unique_ptr<DbConnectionIf>()> s_connectionFactory;

  static std::unique_ptr<DbConnectionIf> CreateConnection() {
    if (s_connectionFactory) return s_connectionFactory();
    return DbConnection::CreateConnection();
  }

  static bool Insert(T &entity) {
    const auto *schema = entity.GetSchema();
    if (!schema) return false;

    std::string cols;
    std::string placeholders;
    std::vector<std::string> params;
    int idx = 1;
    bool first = true;

    for (const auto &col : schema->GetColumns()) {
      if (col->GetFlags() & ColumnFlags::IgnoreOnWrite) continue;

      if (!first) { cols += ", "; placeholders += ", "; }
      cols += col->GetName();
      placeholders += "$" + std::to_string(idx++);
      first = false;

      std::string strVal;
      switch (col->GetType()) {
        case ColumnType::Integer: { auto v = entity.GetInt(col->GetName());    if (v) strVal = std::to_string(*v); break; }
        case ColumnType::Boolean: { auto v = entity.GetBool(col->GetName());   if (v) strVal = *v ? "true" : "false"; break; }
        case ColumnType::Float:   { auto v = entity.GetFloat(col->GetName());  if (v) strVal = std::to_string(*v); break; }
        case ColumnType::Text:    { auto v = entity.GetString(col->GetName()); if (v) strVal = *v; break; }
      }
      params.push_back(std::move(strVal));
    }

    if (idx == 1) return false;

    const std::string query = "INSERT INTO " + schema->GetTableName() + " (" + cols + ") VALUES (" + placeholders + ") RETURNING id";

    unsigned char sha1[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(query.c_str()), query.size(), sha1);
    std::ostringstream stmtName;
    stmtName << std::hex << std::setfill('0');
    for (unsigned char byte : sha1) stmtName << std::setw(2) << static_cast<int>(byte);

    auto conn = CreateConnection();
    if (!conn) return false;

    auto rawResult = conn->GetAll(query, stmtName.str(), params);
    if (!rawResult) return false;

    const auto &data = (*rawResult)->GetData();
    if (data.empty() || data[0].empty()) return false;

    entity.Set("id", std::stoi(data[0][0]));
    return true;
  }

  static bool Update(const T &entity) {
    const auto *schema = entity.GetSchema();
    if (!schema) return false;

    const auto id = entity.Id();
    if (!id) return false;

    std::string assignments;
    std::vector<std::string> params;
    int idx = 1;
    bool first = true;

    for (const auto &col : schema->GetColumns()) {
      if (col->GetFlags() & ColumnFlags::IgnoreOnWrite) continue;

      if (!first) assignments += ", ";
      assignments += col->GetName() + " = $" + std::to_string(idx++);
      first = false;

      std::string strVal;
      switch (col->GetType()) {
        case ColumnType::Integer: { auto v = entity.GetInt(col->GetName());    if (v) strVal = std::to_string(*v); break; }
        case ColumnType::Boolean: { auto v = entity.GetBool(col->GetName());   if (v) strVal = *v ? "true" : "false"; break; }
        case ColumnType::Float:   { auto v = entity.GetFloat(col->GetName());  if (v) strVal = std::to_string(*v); break; }
        case ColumnType::Text:    { auto v = entity.GetString(col->GetName()); if (v) strVal = *v; break; }
      }
      params.push_back(std::move(strVal));
    }

    if (idx == 1) return false;

    params.push_back(std::to_string(*id));
    const std::string query = "UPDATE " + schema->GetTableName() + " SET " + assignments + " WHERE id = $" + std::to_string(idx);

    unsigned char sha1[SHA_DIGEST_LENGTH];
    SHA1(reinterpret_cast<const unsigned char *>(query.c_str()), query.size(), sha1);
    std::ostringstream stmtName;
    stmtName << std::hex << std::setfill('0');
    for (unsigned char byte : sha1) stmtName << std::setw(2) << static_cast<int>(byte);

    auto conn = CreateConnection();
    if (!conn) return false;

    return conn->Exec(query, stmtName.str(), params);
  }

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

    auto conn = CreateConnection();
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

    auto conn = CreateConnection();
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
