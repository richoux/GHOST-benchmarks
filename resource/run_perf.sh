#!/bin/bash

sudo perf record bin/resource protoss 100 > /dev/null ; bin/resource terran 100 > /dev/null ; bin/resource zerg 100 > /dev/null

HOST=$(hostname -fs)
mv perf.data scripts/profiling/perf/$HOST/$(date +%y-%m-%d_%H-%M).data
