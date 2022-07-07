# GHOST Benchmarks
Tested with GHOST v2

## List of benchmarks
### Classic benchmarks
- Killer Sudoku (satisfaction, permutation, constraints: All-Different and Linear-Equation)
- Knapsack (optimization, non-permutation, constraint: Linear-Less-Than-or-Equals-To)
- AllDiff Knapsack (optimization, non-permutation, constraints: All-Different and Linear-Less-Than-or-Equals-To)
- 0-1 Multidimensional Knapsack (satisfaction or optimization, non-permutation, constraint: Linear-Less-Than-or-Equals-To and Linear-Greater-Than-or-Equals-To)
- Magic Square (satisfaction, permutation, constraint: Linear-Equation)
- Min Magic Square (optimization, permutation, constraint: Linear-Equation)
- Sudoku (satisfaction, permutation, constraint: All-Different)
- Vertex Cover (satisfaction, non-permutation, constraints: Linear-Less-Than-or-Equals-To and Linear-Greater-Than-or-Equals-To)

### StarCraft-related benchmarks
Work in progress: Build Order, Resource, Target Selection, Wall-in.

## Commands

Compile with
```shell
$> ./make
```
Important: GHOST v2 must be installed first.

Run all classic benchmarks (except 0-1 multidimensional knapsack) with
```shell
$> ./test_all.sh
```
