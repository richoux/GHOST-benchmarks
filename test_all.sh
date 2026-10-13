#!/bin/bash

echo
echo "============================="
echo "STARCRAFT BUILD ORDER"
echo "============================="
(cd build_order && ./bin/build_order data/10k_games/PvT/GG100.short 2> /dev/null)

echo
echo "============================="
echo "KILLER SUDOKU"
echo "============================="
(cd killer_sudoku && ./bin/killer_sudoku 1 2> /dev/null)

echo
echo "============================="
echo "KNAPSACK"
echo "============================="
(cd knapsack && ./bin/knapsack  2> /dev/null && ./bin/alldiff_knapsack 2> /dev/null)

echo
echo "============================="
echo "MAGIC SQUARE"
echo "============================="
(cd magic_square && ./bin/magic_square 5 2> /dev/null && ./bin/min_magic_square 5 2> /dev/null && ./bin/magic_square 5 1 2> /dev/null && ./bin/min_magic_square 5 1 2> /dev/null)

echo
echo "============================="
echo "MAGIC HEXAGON"
echo "============================="
(cd magic_hexagon && ./bin/magic_hexagon 3 2> /dev/null && ./bin/min_magic_hexagon 3 2> /dev/null && ./bin/magic_hexagon 4 1 2> /dev/null && ./bin/min_magic_hexagon 4 1 2> /dev/null)

# (cd multidimensional_knapsack && ./bin/multidimensional_knapsack instances_gk/gk01.txt 2> /dev/null) #commented out because it takes 1 minute

echo
echo "============================="
echo "N-QUEENS"
echo "============================="
(cd n-queens && ./bin/nqueens_permut 2> /dev/null)

echo
echo "============================="
echo "QAP"
echo "============================="
(cd qap && ./bin/qap_permut instances/tai12a.dat 2> /dev/null)

echo
echo "============================="
echo "STARCRAFT RESOURCE ALLOCATION"
echo "============================="
(cd resource  && ./bin/resource 0 100 1 2> /dev/null)

echo
echo "============================="
echo "SUDOKU"
echo "============================="
(cd sudoku && ./bin/sudoku 2> /dev/null && ./bin/sudoku 3 1 2> /dev/null)

echo
echo "============================="
echo "STARCRAFT TARGET SELECTION"
echo "============================="
(cd target  && ./bin/target 1 2> /dev/null)

echo
echo "============================="
echo "TSP"
echo "============================="
(cd tsp && ./bin/tsp_permut instances/p01.tsp 2> /dev/null)

echo
echo "============================="
echo "TTP"
echo "============================="
(cd ttp && ./bin/ttp_opt instances/J1_2026.txt 2> /dev/null)

echo
echo "============================="
echo "VERTEX COVER"
echo "============================="
(cd vertex_cover && ./bin/vertex_cover_20 2> /dev/null && ./bin/vertex_cover_20 1 2> /dev/null)

echo
echo "============================="
echo "STARCRAFT WALL-IN"
echo "============================="
(cd wall-in && ./bin/wallin 1 2> /dev/null)
