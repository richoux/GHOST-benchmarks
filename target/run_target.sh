#!/bin/bash

FILE=max_damage_ratio
MINUTE=$(date +%H_%M)

HOST=$(hostname -fs)
DATE=$(date +%y-%m-%d_%H-%M)
ITER=100

for i in {1..$ITER}
do
    if [ $# -eq 0 ]; then
	echo "################" >> results/$HOST/"$DATE"_$FILE.txt
	./bin/target >> results/$HOST/"$DATE"_$FILE.txt
	#echo >> results/$FILE
    else
	bin/target $1 $2 >> results/$HOST/"$DATE"_$FILE-$1-$2.txt
    fi
done
