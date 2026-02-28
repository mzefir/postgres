#pragma once

#include <string>

namespace Zef::Template {

class Hello {
public:
  Hello() = default;
  std::string Greet() const;
};
} // namespace Zef::Template