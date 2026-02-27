#pragma once

namespace Zef::Orm {

template<typename T>
class Repository {
public:
  static Repository<T> &GetInstance() {
    static Repository<T> instance;
    return instance;
  }

  Repository(const Repository &) = delete;
  Repository &operator=(const Repository &) = delete;

private:
  Repository() = default;
  ~Repository() = default;
};

} // namespace Zef::Orm
