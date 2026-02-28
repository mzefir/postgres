#include <testing.hpp>

#include "Hello.hpp"

using namespace ::testing;

namespace Zef::Testing {

class HelloTest : public Test {
protected:
  void SetUp() override {}
  void TearDown() override {}
};

TEST_F(HelloTest, IsGreetEqualTo_Hello) {
  Template::Hello hello;
  EXPECT_EQ(hello.Greet(), "Hello!");
}

} // namespace Zef::Testing