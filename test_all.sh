#!/bin/bash

(cd killer_sudoku && ./bin/killer_sudoku 1 2> /dev/null)
(cd knapsack && ./bin/knapsack  2> /dev/null && ./bin/alldiff_knapsack 2> /dev/null)
(cd magic_square && ./bin/magic_square 5 2> /dev/null && ./bin/min_magic_square 5 2> /dev/null && ./bin/magic_square 5 1 2> /dev/null && ./bin/min_magic_square 5 1 2> /dev/null)
(cd sudoku && ./bin/sudoku 2> /dev/null && ./bin/sudoku 3 1 2> /dev/null)
(cd vertex_cover && ./bin/vertex_cover 2> /dev/null && ./bin/vertex_cover 1 2> /dev/null)
