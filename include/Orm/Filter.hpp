#pragma once

#include <string>

namespace Zef::Orm
{

struct Filter
{
  std::string Field;
  std::string Op;
  std::string Value;
};

} // namespace Zef::Orm
