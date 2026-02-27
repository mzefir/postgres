#include "Hello.hpp"
#include <iostream>

using namespace Zef::Template;

int main() {
  Hello hello;
  std::cout << hello.Greet() << std::endl;
}