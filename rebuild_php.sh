#!/usr/bin/env bash

mkdir build > /dev/null 2>&1
cd build
rm -rf *
cmake .. -DPHP=ON
cmake --build .
#cmake .. -DPHP=ON --debug-output
#cmake --build . --verbose
cp -r * ../libdb/libdb/Generated
