#!/bin/bash

(cd killer_sudoku && ./bin/killer_sudoku 1 2> /dev/null)
(cd knapsack && ./bin/knapsack  2> /dev/null && ./bin/alldiff_knapsack 2> /dev/null)
(cd magic_square && ./bin/magic_square 5 2> /dev/null && ./bin/min_magic_square 5 2> /dev/null && ./bin/magic_square 5 1 2> /dev/null && ./bin/min_magic_square 5 1 2> /dev/null)
# (cd multidimensional_knapsack && ./bin/multidimensional_knapsack instances_gk/gk01.txt 2> /dev/null) #commented out because it takes 1 minute
(cd n-queens && ./bin/nqueens_permut 2> /dev/null)
(cd qap && ./bin/qap_permut instances/tai12a.dat 2> /dev/null)
(cd sudoku && ./bin/sudoku 2> /dev/null && ./bin/sudoku 3 1 2> /dev/null)
(cd tsp && ./bin/tsp_permut instances/p01.tsp 2> /dev/null)
(cd vertex_cover && ./bin/vertex_cover 2> /dev/null && ./bin/vertex_cover 1 2> /dev/null)
