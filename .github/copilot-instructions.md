# Copilot Instructions

## Project Overview

C++20 PostgreSQL ORM library. Production code lives in `include/` (public headers) and `src/` (implementations). Tests use GoogleTest/GMock under `test/`.

## Build & Test

```bash
# First-time setup: copy and edit environment config
cp utils/setup_env_dist utils/setup_env
# edit utils/setup_env to set INSTALL_DIR, DB credentials, etc.

# Configure environment (sets INSTALL_DIR=/opt/zef, REPO_ROOT)
source utils/setup_env

# Build
mkdir build && cd build
cmake -G Ninja -DCMAKE_BUILD_TYPE=Debug ..
ninja -j8

# Install
ninja install

# Run full test suite
./tests-postgres

# Run a single test
./tests-postgres --gtest_filter=ColumnTest.GetName_ReturnsConstructedName

# Build with coverage report
ninja coverage
```

To disable code coverage: `export DISABLE_CODE_COVERAGE="DISABLE_CODE_COVERAGE"` (already set in `setup_env`).

## Architecture

**ORM headers** (`include/Orm/`):
- `Defs.hpp` — `ColumnType` enum, `ColumnFlags` bitmask, bitwise operators
- `ColumnIf.hpp` / `Column.hpp` — Column metadata (name, type, flags)
- `TableSchema.hpp` — Immutable ordered list of columns for a table
- `EntityIf.hpp` / `EntityBase.hpp` — Abstract entity interface + base class with `unordered_map<string, ColumnValue>` storage
- `DbConnectionIf.hpp` / `DbConnection.hpp` — Abstract connection interface + `libpq`-backed implementation with static connection pool
- `DbConnectionParams.hpp` — Connection config struct (host, port, dbName, user, password)
- `RawResultIf.hpp` / `RawResult.hpp` — Abstract result set + concrete `vector<string>` column names + `vector<vector<string>>` rows
- `Repository.hpp` — Template CRUD singleton per entity type; generates SHA1-named prepared statements

**ORM object model:** `DbConnection` manages a static `libpq` connection pool. Call `DbConnection::SetConnectionParams(...)` once at startup and `DbConnection::Shutdown()` at exit. `Repository<T>::GetAll()` / `Save()` / `Delete()` / `Get()` use prepared statements to query/mutate the database and return `std::optional<T>`. `RawResult` stores rows as `vector<vector<string>>` with a parallel column-name vector.

**Implementations** (`src/Orm/`): `Column.cc`, `EntityBase.cc`, `DbConnection.cc`, `RawResult.cc`, `TableSchema.cc`.

**User entities** live in `include/Entity/` and `src/Entity/` (e.g., `Category`), inherit `EntityBase`, and implement `GetTableName()` + `GetSchema()` (static schema initialized via IIFE). Register new `.cc` files under `ProjectAppSrc` in `cmake/Common.cmake`.

**Test infrastructure:**
- `test/src/` — GoogleTest test files
- `test/mock/Orm/` — GMock mocks mirroring the `include/Orm/` structure
- `test/include/testing.hpp` — Unified test header; include this instead of gtest/gmock directly
- `test/include/Mock.hpp` — `Mock<T>::Get(name)` factory for named strict mock singletons
- The test build compiles a static lib (`testlibpostgres`) with `-DUNITTEST=1`, then links it into the test executable. Use `#ifdef UNITTEST` to conditionally expose internals.

**cmake/Common.cmake** — Defines `ProjectOrmSrc`, `ProjectAppSrc`, and `TestsSrc` lists. **New `.cc` files and test files must be added here.**

**cmake/FetchContent.cmake** — External deps: `zeflogger` v1.4.3, `googletest` v1.14.0, `PostgreSQL::PostgreSQL`, `OpenSSL::Crypto`.

## Key Conventions

**Namespaces:** ORM library uses `Zef::Orm`; user entities use `Zef::Entity`; test mocks use `Zef::Testing::Orm`.

**Interface pattern:** Abstract base classes named `*If` (e.g., `DbConnectionIf`). Mock implementations in `test/mock/` follow the `*Mock` naming convention and mirror the `include/` subdirectory structure.

**Dependency injection:** Classes accept dependencies as `std::shared_ptr<InterfaceType>`.

**Error handling:** Query methods return `std::optional`, with `std::nullopt` on failure. No exceptions are thrown by ORM code.

**Member variables:** Prefixed with `m_` (e.g., `m_conn`, `m_columnNames`).

**`ColumnFlags` bitmask:** Flags are combined with `operator|` and tested with `operator&`, both defined in `Defs.hpp`.

**Naming:** PascalCase for classes and methods. Source files use `.cc` / `.hpp`.

**Formatting:** Enforced by `.clang-format`. Key settings: 160-char column limit, 2-space indent, `PointerAlignment: Right`, `#pragma once` for header guards.

```bash
# Format the whole repository
utils/format_repository.sh

# Format only staged changes
utils/format_repository.sh --short
```

**CMake targets:**
- `postgres` — main executable
- `testlibpostgres` — static lib built with `-DUNITTEST=1` for tests
- `tests-postgres` — test executable
- `zefpostgres` / `zefpostgresd` — installable release/debug libs
