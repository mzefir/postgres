# Assign different values based on the build type
if(CMAKE_BUILD_TYPE STREQUAL "Debug")
  set(DebugAppend "d")
else()
  set(DebugAppend "")
endif()

## testing section
include(FetchContent)
set(INSTALL_GTEST OFF)

FetchContent_Declare(
  "zeflogger${DebugAppend}"
  GIT_REPOSITORY https://gitlab.com/sidobado/logger.git
  GIT_TAG 1.2
)
FetchContent_Declare(
  googletest
  GIT_REPOSITORY https://github.com/google/googletest.git
  GIT_TAG v1.14.0
)

set(gtest_force_shared_crt ON CACHE BOOL "" FORCE)
FetchContent_MakeAvailable("zeflogger${DebugAppend}")
FetchContent_MakeAvailable(googletest)

find_package(PostgreSQL REQUIRED)

set(ProjectExternalLibraries
  "zeflogger${DebugAppend}"
  PostgreSQL::PostgreSQL
)
