#pragma once

#include "Orm/ColumnIf.hpp"
#include <string>

namespace Zef::Orm {

class Column : public ColumnIf {
public:
  Column() = delete;
  Column(const Column &) = delete;
  Column &operator=(const Column &) = delete;
  Column(Column &&) = delete;
  Column &operator=(Column &&) = delete;

  Column(std::string name, ColumnType type, ColumnFlags flags);
  ~Column() override = default;

  const std::string &GetName() const override;
  ColumnType GetType() const override;
  ColumnFlags GetFlags() const override;

private:
  std::string m_name;
  ColumnType m_type;
  ColumnFlags m_flags;
};

} // namespace Zef::Orm
