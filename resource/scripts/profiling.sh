#!/bin/bash

DATE=$(date +%d-%m-%y_%H-%M)

sudo ls

for i in {1..100}
do
    ../bin/resource terran 1 &
    sleep 0.$[ ( $RANDOM % 9 )  + 1 ]s
    sudo gdb -batch -x debug.commands -p $! >> profiling/profiling_$DATE.txt
    sleep 0.$[ ( $RANDOM % 9 )  + 1 ]s
    sudo gdb -batch -x debug.commands -p $! >> profiling/profiling_$DATE.txt
    sleep 0.$[ ( $RANDOM % 9 )  + 1 ]s
    sudo gdb -batch -x debug.commands -p $! >> profiling/profiling_$DATE.txt
    sleep 0.$[ ( $RANDOM % 9 )  + 1 ]s
    sudo gdb -batch -x debug.commands -p $! >> profiling/profiling_$DATE.txt
done;

grep ghost profiling/profiling_$DATE.txt > profiling/resume_$DATE.txt
./analyse_profiling.rb profiling/resume_$DATE.txt > profiling/percent_$DATE.txt
