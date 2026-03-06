set(CMakeMinVersion "3.18")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ProjectTitle "ZefPostgres")
set(ProjectDescription "A lightweight C++20 ORM library for PostgreSQL.")
set(ProjectVersion "1.3.2")
set(ProjectExecutable "postgres")

# Project source files, insert only *.cc files. Do not add main.cc file but main.cc must be present in main directory.
# 
# Those files will be used in 2 subprojects:
# - to build an executable of application
# - to build a library which is included as dependency for tests subproject
set(ProjectOrmSrc
  "src/Orm/Column.cc"
  "src/Orm/EntityBase.cc"
  "src/Orm/DbConnection.cc"
  "src/Orm/RawResult.cc"
  "src/Orm/TableSchema.cc"
)

set(ProjectAppSrc
  "src/Entity/Category.cc"
)

# Tests source files
set(TestsSrc
  "test/src/Orm/ColumnTest.cc"
  "test/src/Orm/TableSchemaTest.cc"
  "test/src/Orm/RepositoryTest.cc"
)