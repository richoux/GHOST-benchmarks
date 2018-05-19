#!/bin/bash

FILE=max_damage_ratio
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

for i in {1..$ITER}
do
    if [ $# -gt 1 ]; then
	bin/target $1 $2 >> results/$HOST/"$DATE"_$FILE-$1-$2.txt
    else
	echo "################" >> results/$HOST/"$DATE"_$FILE.txt
	./bin/target >> results/$HOST/"$DATE"_$FILE.txt
	#echo >> results/$FILE
    fi
done
