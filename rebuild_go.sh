#!/usr/bin/env bash

mkdir build > /dev/null 2>&1
cd build
rm -rf *
cmake .. -DPYTHON=OFF -DGOLANG=ON
cmake --build .