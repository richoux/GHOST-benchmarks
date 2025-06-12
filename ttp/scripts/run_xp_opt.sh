#!/bin/bash

data="$1"
datafile="$(basename "${data%.*}")"
file=$(date +%Y_%m_%d-%H_%M_%S-"$datafile"-8_cores)

for i in {1..10}
do
		echo "Run $i"
		../bin/ttp_opt "$data" 1 1>> "../results/$file" 2> /dev/null
done
