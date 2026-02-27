#include "Hello.hpp"
#include "DummyIf.hpp"
#include <memory>

namespace Zef::Template {

std::string Hello::Greet() const { return "Hello!"; }

std::string Hello::UseDummy(std::shared_ptr<DummyIf> dummy) { return dummy->Foo(); }

} // namespace Zef::Template
