#!/bin/sh
cd "$(dirname "$0")"
cd ../docs

find . -name *.svg | while read svg; do rm -f $svg; done;
rm -rf out/doc

cd ..
mkdocs build
