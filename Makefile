all:
	(cd killer_sudoku && $(MAKE))
	(cd knapsack && $(MAKE))
	(cd magic_square && $(MAKE))
	(cd multidimensional_knapsack && $(MAKE))
	(cd sudoku && $(MAKE))
	(cd vertex_cover && $(MAKE))

bench:
	(cd killer_sudoku && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd knapsack && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd magic_square && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd multidimensional_knapsack && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd sudoku && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd vertex_cover && $(MAKE) MYFLAGS=-DGHOST_BENCH)

clean:
	(cd killer_sudoku && $(MAKE) clean)
	(cd knapsack && $(MAKE) clean)
	(cd magic_square && $(MAKE) clean)
	(cd multidimensional_knapsack && $(MAKE) clean)
	(cd sudoku && $(MAKE) clean)
	(cd vertex_cover && $(MAKE) clean)
