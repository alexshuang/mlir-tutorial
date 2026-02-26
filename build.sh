#!/bin/bash

export MLIR_DIR=$PWD/third_party/llvm-project/install/
mkdir -p build
cd build; cmake ..
make -j4
