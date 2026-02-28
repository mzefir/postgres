# Copilot Instructions

## Project Overview

C++20 PostgreSQL ORM library. Production code lives in `include/` (public headers) and `src/` (implementations). Tests use GoogleTest/GMock under `test/`.

## Build & Test

```bash
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
./tests-postgres --gtest_filter=HelloTest.IsGreetEqualTo_Hello

# Build with coverage report
ninja coverage
```

To disable code coverage: `export DISABLE_CODE_COVERAGE="DISABLE_CODE_COVERAGE"` (already set in `setup_env`).

## Architecture

- **`include/Orm/`** — Public ORM headers: `DatabaseIf.hpp`, `ConnectionIf.hpp`, `RawResultIf.hpp`, `Column.hpp`, `Defs.hpp`, `Repository.hpp`.
- **`src/Orm/`** — Implementations wrapping `libpq` (PostgreSQL C API).
- **`test/src/`** — GoogleTest test files.
- **`test/mock/Orm/`** — GMock mocks mirroring the `include/Orm/` structure.
- **`test/include/testing.hpp`** — Unified test header; include this instead of including gtest/gmock directly.
- **`cmake/Common.cmake`** — Defines `ProjectSrc` and `TestsSrc` lists. **New `.cc` files and test files must be added here.**
- **`cmake/FetchContent.cmake`** — External deps: `zeflogger` v1.2, `googletest` v1.14.0, `PostgreSQL::PostgreSQL`.

**ORM object model:** `Database` creates `Connection` objects via `CreateConnection()`. `Connection` executes queries and returns `std::optional<std::unique_ptr<RawResultIf>>`. `RawResult` stores rows as `vector<vector<string>>` with a parallel column-name vector. `Repository<T>` is a per-type singleton (currently a scaffold).

`Connection` has a protected constructor and declares `Database` as a friend — it is only instantiable through `Database::CreateConnection()`.

The test build compiles a static lib (`testlibpostgres`) with `-DUNITTEST=1`, then links it into the test executable. This flag can be used to conditionally expose internals for testing.

## Key Conventions

**Namespaces:** Production ORM code uses `Zef::Orm`; test mocks use `Zef::Testing::Orm`.

**Interface pattern:** Abstract base classes named `*If` (e.g., `ConnectionIf`). Mock implementations in `test/mock/` follow the `*Mock` naming convention and mirror the `include/` subdirectory structure.

**Dependency injection:** Classes accept dependencies as `std::shared_ptr<InterfaceType>`.

**Error handling:** Query methods return `std::optional`, with `std::nullopt` on failure. No exceptions are thrown by ORM code.

**Member variables:** Prefixed with `m_` (e.g., `m_conn`, `m_columnNames`).

**`ColumnFlags` bitmask:** Flags are combined with `operator|` and tested with `operator&`, both defined in `Defs.hpp`.

**Naming:** PascalCase for classes and methods. Source files use `.cc` / `.hpp`.

**Formatting:** Enforced by `.clang-format`. Key settings: 160-char column limit, 2-space indent, `PointerAlignment: Right`, `#pragma once` for header guards.

```bash
# Format the whole repository
utils/format_repository.sh
```

**CMake targets:**
- `postgres` — main executable
- `testlibpostgres` — static lib built with `-DUNITTEST=1` for tests
- `tests-postgres` — test executable
- `libpostgres` / `libpostgresd` — installable release/debug libs
