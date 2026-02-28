#pragma once

#include "Orm/EntityIf.hpp"
#include <memory>
#include <vector>

namespace Zef::Orm {

class RepositoryIf {
public:
  virtual ~RepositoryIf() = default;

  virtual std::vector<std::shared_ptr<EntityIf>> GetAll() = 0;
};

} // namespace Zef::Orm
