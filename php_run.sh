#!/usr/bin/env bash

cp build/libdb.so /usr/local/Cellar/php/7.3.4/pecl/20180731/
cp src/php/cpp_connect.php build/
cd build
php cpp_connect.php