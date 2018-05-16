#!/bin/bash

#$1 = terran/protoss/zerg
#$2 = number of runs

HOST=$(hostname -fs)
DATE=$(date +%y-%m-%d_%H-%M)
ITER=1000

if [ $# -eq 0 ]; then
    bin/resource protoss $ITER > scripts/bench/$HOST/"$DATE"_protoss_$ITER.txt
    bin/resource terran $ITER > scripts/bench/$HOST/"$DATE"_terran_$ITER.txt
    bin/resource zerg $ITER > scripts/bench/$HOST/"$DATE"_zerg_$ITER.txt
    exit 0
else
    bin/resource $1 $2 > scripts/bench/$HOST/"$DATE"_$1_$2.txt
fi
