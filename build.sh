#!/usr/bin/env bash

num_samples=$1
resolution=$2

if [[ ! -d build ]]; then
    mkdir -p build
    cd build
    cmake ..
    cd ..
fi

cd build
make -j || exit 1
cd ..

mkdir -p output