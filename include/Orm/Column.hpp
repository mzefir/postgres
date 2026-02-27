#pragma once

#include "Orm/Defs.hpp"
#include <string>

namespace Zef::Orm {

class Column {
public:
  Column() = delete;
  Column(const Column &) = delete;
  Column &operator=(const Column &) = delete;
  Column(Column &&) = delete;
  Column &operator=(Column &&) = delete;

  Column(std::string name, ColumnType type, ColumnFlags flags);
  ~Column() = default;

  const std::string &GetName() const;
  ColumnType GetType() const;
  ColumnFlags GetFlags() const;

private:
  std::string m_name;
  ColumnType m_type;
  ColumnFlags m_flags;
};

} // namespace Zef::Orm
