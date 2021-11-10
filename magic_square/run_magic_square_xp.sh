#!/bin/bash

date=$(date +%Y-%m-%d)
path=results/"$date"/"$HOST"
mkdir -p "$path"

for i in {1..100}; do
    ./bin/magic_square 20 1 8 >> ./"$path"/magic_square_20_8cores.txt
    ./bin/magic_square 30 1 8 >> ./"$path"/magic_square_30_8cores.txt
    ./bin/magic_square 40 1 8 >> ./"$path"/magic_square_40_8cores.txt
done
