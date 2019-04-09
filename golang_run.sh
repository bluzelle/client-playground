#!/usr/bin/env bash

#export GOPATH=$(pwd)
export DYLD_LIBRARY_PATH=$(pwd)/build
export CGO_LDFLAGS="-L$DYLD_LIBRARY_PATH -ldb"

go build

./swig-playground