#!/bin/bash

FILE=max_damage_max_kill_ratio
MINUTE=$(date +%H_%M)

HOST=$(hostname -fs)
DATE=$(date +%y-%m-%d_%H-%M)
ITER=100

if [ $# -eq 1 ]; then
    ITER=$1
fi

if [ $# -eq 3 ]; then
    ITER=$3
fi

for ((i = 1; i <= ITER; i++))
do
    if [ $# -gt 1 ]; then
	bin/target $1 $2 >> results/$HOST/"$DATE"_"$FILE"_$1-$2-$ITER.txt
    else
	echo "################" >> results/$HOST/"$DATE"_"$FILE"_$ITER.txt
	./bin/target >> results/$HOST/"$DATE"_"$FILE"_$ITER.txt
	#echo >> results/$FILE
    fi
done
