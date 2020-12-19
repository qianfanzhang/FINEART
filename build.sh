#!/usr/bin/env bash

cd include/scene
./build.sh || exit 1
cd ../..

if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake .. || exit 2
    cd ..
fi

cd build
make -j || exit 3
cd ..