# Mini ORM for PostgreSQL

## Overview
A lightweight C++20 ORM library for PostgreSQL. It provides connection pooling via `DbConnection`,
a typed `Repository<T>` for querying entities, and a schema/column abstraction built on top of `libpq`.

## How to add this as a library to your project

Add to your CMakeLists.txt:

```cmake
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(DebugAppend "d")
else()
  set(DebugAppend "")
endif()

# Library build for debug target has 'd' postfix
FetchContent_Declare(
  "zefpostgres${DebugAppend}"
  GIT_REPOSITORY ssh://git@localhost/git/zef/cpp-postgres.git
  GIT_TAG 1.0.0
)
```

## How to use

### Setting up the database connection

Call `DbConnection::SetConnectionParams` once at startup, before creating any connections:

```cpp
Zef::Orm::DbConnection::SetConnectionParams(
  "localhost", "5432", "mydb", "myuser", "mypassword"
);
```

Environment variables can be used as a convenient alternative (see `src/main.cc` for a pattern
using `getenv_or`).

### Querying entities

Use `Repository<T>::GetAll()` to fetch all rows for an entity:

```cpp
auto result = Zef::Orm::Repository<Zef::Entity::Category>::GetAll();
if (result) {
  for (const auto &item : *result) {
    // access item fields via item.GetString("column") etc.
    // auto &item in this context is Zef::Entity::Category &item.
  }
}
```

### How to create entity objects

Entities inherit from `Zef::Orm::EntityBase` and must implement `GetTableName()` and `GetSchema()`.

**Header** (`include/Entity/MyEntity.hpp`):

```cpp
#pragma once
#include "Orm/EntityBase.hpp"

namespace Zef::Entity {

class MyEntity : public Zef::Orm::EntityBase {
public:
  MyEntity() = default;
  ~MyEntity() override = default;

  const std::string &GetTableName() const override;
  const Zef::Orm::TableSchema *GetSchema() const override;

private:
  static const std::string s_tableName;
};

} // namespace Zef::Entity
```

**Source** (`src/Entity/MyEntity.cc`):

```cpp
#include "Entity/MyEntity.hpp"
#include "Orm/Column.hpp"

namespace Zef::Entity {

const std::string MyEntity::s_tableName{"my_table"};

const std::string &MyEntity::GetTableName() const { return s_tableName; }

const Zef::Orm::TableSchema *MyEntity::GetSchema() const {
  static const std::unique_ptr<Zef::Orm::TableSchema> s_schema = []() {
    std::vector<std::unique_ptr<Zef::Orm::ColumnIf>> cols;
    cols.push_back(std::make_unique<Zef::Orm::Column>("id",    Zef::Orm::ColumnType::Integer, Zef::Orm::ColumnFlags::IgnoreOnWrite));
    cols.push_back(std::make_unique<Zef::Orm::Column>("title", Zef::Orm::ColumnType::Text,    Zef::Orm::ColumnFlags::Nullable));
    return std::make_unique<Zef::Orm::TableSchema>(s_tableName, std::move(cols));
  }();
  return s_schema.get();
}

} // namespace Zef::Entity
```

Register the new `.cc` file in `cmake/Common.cmake` under `ProjectAppSrc`.

### Shutdown

Call `DbConnection::Shutdown()` at the end of `main()` to close all pooled connections cleanly:

```cpp
Zef::Orm::DbConnection::Shutdown();
```

## How to build

```shell
cp utils/setup_env_dist utils/setup_env
# update utils/setup_env using your favourite editor

source utils/setup_env
mkdir build
cd build

cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
# or
cmake -G Ninja -DCMAKE_BUILD_TYPE=RELEASE ..

ninja
```