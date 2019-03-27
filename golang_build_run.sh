#!/usr/bin/env bash

mkdir -p src/github.com/yarco/swig-playground/libdb
cp build/libdb.go src/github.com/yarco/swig-playground/libdb/

export GOPATH=$(pwd)
export DYLD_LIBRARY_PATH=$(pwd)/build
export CGO_LDFLAGS="-L$DYLD_LIBRARY_PATH -ldb"

cd src/github.com/yarco/swig-playground
go build

./swig-playground