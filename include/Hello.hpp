#pragma once

#include <memory>
#include <string>

#include "DummyIf.hpp"

namespace Zef::Template {

class Hello {
public:
  Hello() = default;
  std::string Greet() const;
  std::string UseDummy(std::shared_ptr<DummyIf> dummy);
};
} // namespace Zef::Template