#!/bin/bash

date=$(date +%Y-%m-%d)
host_name=$(hostname)
path_results=results/"$date"/"$host_name"
mkdir -p "$path_results"

for i in {1..100}; do
    ./bin/magic_square 20 1 8 >> ./"$path_results"/magic_square_20_8cores.txt
    ./bin/magic_square 30 1 8 >> ./"$path_results"/magic_square_30_8cores.txt
    ./bin/magic_square 40 1 8 >> ./"$path_results"/magic_square_40_8cores.txt
done
