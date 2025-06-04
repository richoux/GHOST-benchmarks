#!/bin/bash

teams="$1"
file=$(date +%Y_%m_%d-%H_%M_%S-"$teams"_teams-8_cores)

for i in {1..10}
do
		echo "Run $i"
		../bin/ttp_sat "$teams" 1 1>> "../results/$file" 2> /dev/null
done
