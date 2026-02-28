include(cmake/Common.cmake)
cmake_minimum_required(VERSION ${CMakeMinVersion})

project(${ProjectTitle} VERSION ${ProjectVersion} DESCRIPTION ${ProjectDescription})

include(cmake/FetchContent.cmake)

include_directories(".")
include_directories("include")

add_executable (${ProjectExecutable}
  "src/main.cc" ${ProjectOrmSrc} ${ProjectAppSrc}
)
target_link_libraries(${ProjectExecutable} PRIVATE ${ProjectExternalLibraries})
set_property(TARGET ${ProjectExecutable} PROPERTY CXX_STANDARD 20)

include_directories("test/include")
include_directories("test/mock")

add_library(${ProjectExecutableTestLib} STATIC
  ${ProjectOrmSrc} ${ProjectAppSrc}
)
target_compile_definitions(${ProjectExecutableTestLib} PRIVATE UNITTEST=1)
set_property(TARGET ${ProjectExecutableTestLib} PROPERTY CXX_STANDARD 20)
target_link_libraries(${ProjectExecutableTestLib} PRIVATE ${ProjectExternalLibraries})

add_executable (${TestProjectExecutable}
  "test/src/main.cc" ${TestsSrc}
)
set_property(TARGET ${TestProjectExecutable} PROPERTY CXX_STANDARD 20)
target_link_libraries(${TestProjectExecutable} PRIVATE ${ProjectExecutableTestLib} ${ProjectExternalLibraries} GTest::gtest GTest::gmock)
add_dependencies(${TestProjectExecutable} ${ProjectExecutableTestLib} GTest::gtest GTest::gmock)

## library section
add_library("${ProjectLibrary}${DebugAppend}"
  ${ProjectOrmSrc}
)
target_link_libraries("${ProjectLibrary}${DebugAppend}" PRIVATE ${ProjectExternalLibraries})
target_include_directories("${ProjectLibrary}${DebugAppend}" PUBLIC
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
    $<INSTALL_INTERFACE:include>
)
set(CMAKE_SKIP_INSTALL_ALL_DEPENDENCY true)

install(
  TARGETS "${ProjectLibrary}${DebugAppend}"
  LIBRARY DESTINATION ${CMAKE_BINARY_DIR}/out
  ARCHIVE DESTINATION ${CMAKE_BINARY_DIR}/out
  RUNTIME DESTINATION ${CMAKE_BINARY_DIR}/bin
)

install (CODE "execute_process(COMMAND bash ${CMAKE_SOURCE_DIR}/utils/post_install.sh ${ProjectExecutable} ${ProjectVersion} ${CMAKE_BINARY_DIR}/out)")

if(NOT DEFINED ENV{DISABLE_CODE_COVERAGE} AND NOT "$ENV{DISABLE_CODE_COVERAGE}" STREQUAL "DISABLE_CODE_COVERAGE")
  # Check supported compiler (Clang, GNU and Flang)
  get_property(LANGUAGES GLOBAL PROPERTY ENABLED_LANGUAGES)
  SET(IGNORE_COVERAGE FALSE)
  foreach(LANG ${LANGUAGES})
    if("${CMAKE_${LANG}_COMPILER_ID}" MATCHES "(Apple)?[Cc]lang")
      if("${CMAKE_${LANG}_COMPILER_VERSION}" VERSION_LESS 3)
        SET(IGNORE_COVERAGE TRUE)
      endif()
    elseif(NOT "${CMAKE_${LANG}_COMPILER_ID}" MATCHES "GNU"
          AND NOT "${CMAKE_${LANG}_COMPILER_ID}" MATCHES "(LLVM)?[Ff]lang")
          SET(IGNORE_COVERAGE TRUE)
    endif()
  endforeach()

  if (NOT IGNORE_COVERAGE)
      include(cmake/CodeCoverage.cmake)
      append_coverage_compiler_flags()

      set(CODE_COVERAGE_VERBOSE true)
      set(CMAKE_CXX_FLAGS  "${CMAKE_CXX_FLAGS} -O0")
      set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -O0")

      # optional excludes - None needed here
      set(COVERAGE_EXCLUDES
        "build*"
        "src/main.cc"
        "src/Console/Logger.hpp"
      )

      setup_target_for_coverage_gcovr_html(
        NAME coverage
        EXECUTABLE ${TestProjectExecutable}
        DEPENDENCIES ${TestProjectExecutable} ${ProjectExecutableTestLib}
        BASE_DIRECTORY "./"
      )
  endif()

endif()
