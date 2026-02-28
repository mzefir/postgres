set(CMakeMinVersion "3.18")

set(CMAKE_EXPORT_COMPILE_COMMANDS ON)

set(ProjectTitle "Postgresql ORM C++ Library")
set(ProjectDescription "Project description")
set(ProjectVersion "1.0.0")
set(ProjectExecutable "postgres")
set(ProjectLibrary "libpostgres")


set(TestProjectTitle "${ProjectTitle}Tests")
set(ProjectExecutableTestLib "testlib${ProjectExecutable}")
set(TestProjectExecutable "tests-${ProjectExecutable}") # Executable name for subproject with tests

# Project source files, insert only *.cc files. Do not add main.cc file but main.cc must be present in main directory.
# 
# Those files will be used in 2 subprojects:
# - to build an executable of application
# - to build a library which is included as dependency for tests subproject
set(ProjectSrc
  "src/Hello.cc"
  "src/Orm/Column.cc"
  "src/Orm/EntityBase.cc"
  "src/Orm/Connection.cc"
  "src/Orm/Database.cc"
  "src/Orm/RawResult.cc"
  "src/Orm/TableSchema.cc"
)

# Tests source files
set(TestsSrc
  "test/src/HelloTest.cc"
  "test/src/Orm/ColumnTest.cc"
  "test/src/Orm/TableSchemaTest.cc"
)