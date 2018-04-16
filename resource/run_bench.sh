#!/bin/bash

#$1 = terran/protoss/zerg
#$2 = number of runs

bin/resource $1 $2 > scripts/bench/$1_$2_$(date +%y-%m-%d_%H-%M).txt
