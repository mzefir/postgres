#pragma once

#include "DummyIf.hpp"

namespace Zef::Template {
class Dummy : public DummyIf {
public:
  Dummy() = default;
  ~Dummy() = default;

  std::string Foo() const;
};
} // namespace Zef::Template