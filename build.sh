#!/usr/bin/env bash

if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

cd build
make -j || exit 1
cd ..