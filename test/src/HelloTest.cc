#include "gmock/gmock.h"
#include <gtest/gtest.h>
#include <testing.hpp>

#include "DummyMock.hpp"
#include "Hello.hpp"

using namespace ::testing;

namespace {
const std::string HELLO_DUMMY{"Hello Dummy"};
}

namespace Zef::Testing {

class HelloTest : public Test {
protected:
  void SetUp() override {}

  void TearDown() override { Mock<DummyMock>::Dispose(); }
};

TEST_F(HelloTest, IsGreetEqualTo_Hello) {

  Template::Hello hello;
  EXPECT_EQ(hello.Greet(), "Hello!");
}

TEST_F(HelloTest, IsMockWorking) {
  Template::Hello hello;
  auto dummy = Mock<DummyMock>::Get();

  EXPECT_CALL(*dummy, Foo()).WillOnce(Return(HELLO_DUMMY));
  EXPECT_EQ(hello.UseDummy(dummy), HELLO_DUMMY);
}

} // namespace Zef::Testing