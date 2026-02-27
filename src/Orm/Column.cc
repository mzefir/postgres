#include "Orm/Column.hpp"

namespace Zef::Orm {

Column::Column(std::string name, ColumnType type, ColumnFlags flags)
  : m_name(std::move(name)), m_type(type), m_flags(flags) {}

const std::string &Column::GetName() const { return m_name; }

ColumnType Column::GetType() const { return m_type; }

ColumnFlags Column::GetFlags() const { return m_flags; }

} // namespace Zef::Orm
