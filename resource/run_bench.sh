#!/bin/bash

#$1 = terran/protoss/zerg
#$2 = number of runs

HOST=$(hostname -fs)
DATE=$(date +%y-%m-%d_%H-%M)

if [ $# -eq 0 ]; then
    bin/resource protoss 100 > scripts/bench/$HOST/"$DATE"_protoss_100.txt
    bin/resource terran 100 > scripts/bench/$HOST/"$DATE"_terran_100.txt
    bin/resource zerg 100 > scripts/bench/$HOST/"$DATE"_zerg_100.txt
    exit 0
else
    bin/resource $1 $2 > scripts/bench/$HOST/"$DATE"_$1_$2.txt
fi
