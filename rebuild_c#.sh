#!/usr/bin/env bash

mkdir build > /dev/null 2>&1
cd build
rm -rf *
cmake .. -DCSHARP=ON
cmake --build .
#cmake .. -DCSHARP=ON --debug-output
#cmake --build . --verbose
cp -r * ../libdb/libdb/Generated
