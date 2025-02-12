all:
	(cd killer_sudoku && $(MAKE))
	(cd knapsack && $(MAKE))
	(cd magic_square && $(MAKE))
	(cd multidimensional_knapsack && $(MAKE))
	(cd n-queens && $(MAKE))
	(cd qap && $(MAKE))
	(cd sudoku && $(MAKE))
	(cd tsp && $(MAKE))
	(cd vertex_cover && $(MAKE))

bench:
	(cd killer_sudoku && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd knapsack && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd magic_square && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd multidimensional_knapsack && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd n-queens && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd qap && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd sudoku && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd tsp && $(MAKE) MYFLAGS=-DGHOST_BENCH)
	(cd vertex_cover && $(MAKE) MYFLAGS=-DGHOST_BENCH)

trace:
	(cd killer_sudoku && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd knapsack && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd magic_square && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd multidimensional_knapsack && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd n-queens && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd qap && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd sudoku && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd tsp && $(MAKE) MYFLAGS=-DGHOST_TRACE)
	(cd vertex_cover && $(MAKE) MYFLAGS=-DGHOST_TRACE)

clean:
	(cd killer_sudoku && $(MAKE) clean)
	(cd knapsack && $(MAKE) clean)
	(cd magic_square && $(MAKE) clean)
	(cd multidimensional_knapsack && $(MAKE) clean)
	(cd n-queens && $(MAKE) clean)
	(cd qap && $(MAKE) clean)
	(cd sudoku && $(MAKE) clean)
	(cd tsp && $(MAKE) clean)
	(cd vertex_cover && $(MAKE) clean)
