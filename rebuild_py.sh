#!/usr/bin/env bash

mkdir build > /dev/null 2>&1
cd build
rm -rf *
cmake .. -DPYTHON=ON -DGOLANG=OFF
cmake --build .