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

- **`include/`** — Public headers (`.hpp`). Contains interfaces (`*If.hpp`) and concrete classes.
- **`src/`** — Implementations (`.cc`) and `main.cc`.
- **`test/src/`** — GoogleTest test files.
- **`test/mock/`** — GMock mock classes (`*Mock.hpp`) implementing the `*If` interfaces.
- **`cmake/`** — `Common.cmake` (project config), `FetchContent.cmake` (deps: `zeflogger` v1.2, `googletest` v1.14.0), `CodeCoverage.cmake`.
- **`utils/`** — `setup_env` (env setup), `format_repository.sh`, `generate_docs.sh`.

The test build compiles a static lib (`testlibpostgres`) with `-DUNITTEST=1`, then links it into the test executable. This flag can be used to conditionally expose internals for testing.

## Key Conventions

**Namespaces:** Production code uses `Zef::Template`; test mocks use `Zef::Testing`.

**Interface pattern:** Abstract base classes named `*If` (e.g., `DummyIf`) are used for dependency injection. Mock implementations in `test/mock/` follow the `*Mock` naming convention.

**Dependency injection:** Classes accept dependencies as `std::shared_ptr<InterfaceType>`.

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
