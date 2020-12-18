#!/usr/bin/env bash

./build.sh || exit 1

mkdir -p output
scene_name=$1
num_samples=$2
resolution=$3

echo
bin/BINART $scene_name $num_samples $resolution 2>&1 | tee output/$scene_name.log