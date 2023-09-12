# GHOST Benchmarks
Tested with GHOST v2.8

## List of benchmarks
### Classic benchmarks
- Killer Sudoku (satisfaction, permutation, constraints: All-Different and Linear-Equation-Equals-To)
- Knapsack (optimization, non-permutation, constraints: Linear-Less-Than-or-Equals-To)
- AllDiff Knapsack (optimization, non-permutation, constraints: All-Different and Linear-Less-Than-or-Equals-To)
- 0-1 Multidimensional Knapsack (satisfaction or optimization, non-permutation, constraints: Linear-Less-Than-or-Equals-To and Linear-Greater-Than-or-Equals-To)
- Magic Square (satisfaction, permutation, constraints: Linear-Equation-Equals-To)
- Min Magic Square (optimization, permutation, constraints: Linear-Equation-Equals-To)
- Quadratic Assignment Problem (optimization, permutation, no constraints). Require GHOST v2.8 or above.
- AllDiff Quadratic Assignment Problem (optimization, non-permutation, constraints: All-Different)
- Sudoku (satisfaction, permutation, constraints: All-Different)
- N-Queens (satisfaction, permutation, constraints: custom diagonal checks)
- AllDiff N-Queens (satisfaction, non-permutation, constraints: custom diagonal checks, All-Different)
- Traveling Salesman Problem (optimization, permutation, no constraints). Require GHOST v2.8 or above.
- AllDiff Traveling Salesman Problem (optimization, non-permutation, constraints: All-Different)
- Vertex Cover (satisfaction, non-permutation, constraints: Linear-Less-Than-or-Equals-To and Linear-Greater-Than-or-Equals-To)

### StarCraft-related benchmarks
Work in progress: Build Order, Resource, Target Selection, Wall-in.

## Commands

Compile with
```shell
$> ./make
```
Important: GHOST v2.5 or above must be installed. The Quadratic Assignment Problem (QAP) and the Traveling Salesman Problem (TSP) requires at least GHOST v2.8.

Run all classic benchmarks (except 0-1 multidimensional knapsack) with
```shell
$> ./test_all.sh
```
