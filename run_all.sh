#!/usr/bin/env bash

./build.sh || exit 1

run() {
    echo
    scene_name=$1
    bin/BINART $scene_name $num_samples $resolution 2>&1 | tee output/$scene_name.log
}

mkdir -p output
num_samples=$1
resolution=$2

# run earth_in_box
# run girl_in_box
# run dragon_in_box
# run house
run glass
# run steampunk