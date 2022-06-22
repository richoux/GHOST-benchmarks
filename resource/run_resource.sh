#!/bin/bash

#$1 = random or empty

RANDOM=0
RANDOMFILE=""
if [ $# -eq 1 ] && [ "$1" == "random" ]; then
		RANDOM=1
		RANDOMFILE="_random"
fi

HOST=$(hostname -fs)
DATE=$(date +%y-%m-%d_%H-%M)
ITER=100
TIMEOUT=100 #ms
CORES=$(lscpu | grep -E 'CPU\(s\)' | grep -v '-' | awk -F': ' '{print $2}' | tr -d [:blank:])

bin/resource protoss $RANDOM $TIMEOUT $ITER > results/$HOST/"$DATE"_protoss_"$ITER"_"$TIMEOUT"ms"$RANDOMFILE".txt
bin/resource terran $RANDOM $TIMEOUT $ITER > results/$HOST/"$DATE"_terran_"$ITER"_"$TIMEOUT"ms"$RANDOMFILE".txt
bin/resource zerg $RANDOM $TIMEOUT $ITER > results/$HOST/"$DATE"_zerg_"$ITER"_"$TIMEOUT"ms"$RANDOMFILE".txt
bin/resource protoss $RANDOM $TIMEOUT $ITER 1 > results/$HOST/"$DATE"_protoss_"$ITER"_"$TIMEOUT"ms_"$CORES"cores"$RANDOMFILE".txt
bin/resource terran $RANDOM $TIMEOUT $ITER 1 > results/$HOST/"$DATE"_terran_"$ITER"_"$TIMEOUT"ms_"$CORES"cores"$RANDOMFILE".txt
bin/resource zerg $RANDOM $TIMEOUT $ITER 1 > results/$HOST/"$DATE"_zerg_"$ITER"_"$TIMEOUT"ms_"$CORES"cores"$RANDOMFILE".txt
exit 0
