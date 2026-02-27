#pragma once

#include <string>

namespace Zef::Template {
class DummyIf {
public:
  DummyIf() = default;
  virtual ~DummyIf() = default;

  virtual std::string Foo() const = 0;
};
} // namespace Zef::Template