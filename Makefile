all:
	(cd killer_sudoku && $(MAKE))
	(cd knapsack && $(MAKE))
	(cd magic_square && $(MAKE))
	(cd sudoku && $(MAKE))
	(cd vertex_cover && $(MAKE))

clean:
	(cd killer_sudoku && $(MAKE) clean)
	(cd knapsack && $(MAKE) clean)
	(cd magic_square && $(MAKE) clean)
	(cd sudoku && $(MAKE) clean)
	(cd vertex_cover && $(MAKE) clean)
