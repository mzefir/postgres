#pragma once

#include <testing.hpp>

#include "DummyIf.hpp"

namespace Zef::Testing {
class DummyMock : public Template::DummyIf {
public:
  MOCK_METHOD(std::string, Foo, (), (const, override));
};
} // namespace Zef::Testing