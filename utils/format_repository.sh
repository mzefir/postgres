#!/bin/bash
cd "$(dirname "$0")"
cd ..

#TO DO:
#[ ] clang-format-19 may be named as clang-format
#[ ] clang-format may be in other version, need to check is it working for older and newer version
#[ ] project should store what was modified in .clang-format file located in repository root path

CLANG_FORMAT=clang-format-19
#CLANG_FORMAT=clang-format

if [[ "$1" == "--short" ]]; then
  echo "Formatting only files that are staged for commit...";
  git status --short | awk '{print $2}' | grep -E "\.hpp|\.cc$" | \
  grep -vE "^\./.cache/" | \
  grep -vE "^\./build/" | \
  grep -vE "^\./release/" | \
    while read filename; \
    do \
      $CLANG_FORMAT -i --style=file $filename; \
      echo "... $filename"; \
    done;
else
  echo "Formatting all files in the repository...";
  find . | grep -E "\.hpp|\.cc$" | \
  grep -vE "^\./.cache/" | \
  grep -vE "^\./build/" | \
  grep -vE "^\./release/" | \
    while read filename; \
    do \
      $CLANG_FORMAT -i --style=file $filename; \
      echo "... $filename"; \
    done;
fi

echo "Done formatting files.";
