#pragma once

#include <gmock/gmock.h>
#include <map>
#include <memory>

using namespace ::testing;

namespace Zef::Testing {

template <class T> class Mock {
public:
  static std::shared_ptr<T> Get(const std::string &name) {
    if (mocks.find(name) == mocks.end()) {
      createMock(name);
    }
    return mocks[name];
  }

  static std::shared_ptr<T> Get() { return Get("default"); }

  static void Dispose() {
    for (auto &mock : mocks) {
      mock.second.reset();
    }
    mocks.clear();
  }

protected:
  static void createMock(const std::string name) {
    auto ptr = new StrictMock<T>();
    mocks[name] = std::shared_ptr<T>{ptr};
  }

private:
  static std::map<std::string, std::shared_ptr<T>> mocks;
};

template <class T> std::map<std::string, std::shared_ptr<T>> Mock<T>::mocks;

} // namespace Zef::Testing