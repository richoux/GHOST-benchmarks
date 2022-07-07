#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

#include <ghost/solver.hpp>

#include "builder_killer_sudoku.hpp"
#include "print_sudoku.hpp"

using namespace ghost;
using namespace std;
using namespace std::literals::chrono_literals;

bool alldiff_concept( const vector<int>& var )
{
	// We assume our k variables can take values in [1, k]
	vector<bool> bitvec( var.size(), false );

	// Returns false if and only if we have two variables sharing the same value, 
	for( int i = 0 ; i < static_cast<int>( var.size() ) ; ++i )
		if( !bitvec[ var[i]-1 ] )
			bitvec[ var[i]-1 ] = true;
		else
			return false;
	
	return true;	
}

bool check_cage( const std::vector<int>& solution, int cage_number, int total, int var1, int var2 )
{
	if( solution[var1] + solution[var2] != total )
	{
		std::cout << "Killer Sudoku - Error cage " << cage_number << ": " << "v[" << var1 << "]+v[" << var2 << "] = "
		          << solution[var1] << "+" << solution[var2] << " != " << total << "\n";
		return false;
	}
	return true;
}

bool check_cage( const std::vector<int>& solution, int cage_number, int total, int var1, int var2, int var3 )
{
	if( solution[var1] + solution[var2] + solution[var3] != total )
	{
		std::cout << "Killer Sudoku - Error cage " << cage_number << ": " << "v[" << var1 << "]+v[" << var2 << "]+v[" << var3 << "] = "
		          << solution[var1] << "+" << solution[var2] << "+" << solution[var3] << " != " << total << "\n";
		return false;
	}
	return true;
}

bool check_cage( const std::vector<int>& solution, int cage_number, int total, int var1, int var2, int var3, int var4 )
{
	if( solution[var1] + solution[var2] + solution[var3] + solution[var4] != total )
	{
		std::cout << "Killer Sudoku - Error cage " << cage_number << ": " << "v[" << var1 << "]+v[" << var2 << "]+v[" << var3 << "]+v[" << var4 << "] = "
		          << solution[var1] << "+" << solution[var2] << "+" << solution[var3] << "+" << solution[var4] << " != " << total << "\n";
		return false;
	}
	return true;
}

bool check_solution( const vector<int>& solution )
{
	int nb_vars = solution.size();
	int size_side = static_cast<int>( std::sqrt( nb_vars ) );
	int size_side_small_square = static_cast<int>( std::sqrt( size_side ) );

	vector<int> partial_sol( size_side );
	bool success = true;
	
	// Rows
	for( int i = 0; i < size_side; ++i )
	{
		std::copy( solution.begin() + ( i * size_side ),
		           solution.begin() + ( ( i + 1 ) * size_side ),
		           partial_sol.begin() );

		if( !alldiff_concept( partial_sol ) )
		{
			cout << "Killer Sudoku - Error in row " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "\n";
			success = false;
		}
	}

	// Columns
	for( int i = 0; i < size_side; ++i )
	{
		for( int j = 0; j < size_side; ++j )
			partial_sol[j] = solution[ j * size_side + i ];

		if( !alldiff_concept( partial_sol ) )
		{
			cout << "Killer Sudoku - Error in column " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "\n";
			success = false;
		}
	}

	// Squares
	for( int i = 0; i < size_side_small_square; ++i )
		for( int j = 0; j < size_side_small_square; ++j )
		{
			for( int k = 0; k < size_side_small_square; ++k )
				for( int l = 0; l < size_side_small_square; ++l )
				partial_sol[ k * size_side_small_square + l ] = solution[ i * ( size_side_small_square * size_side ) + j * size_side_small_square + k * size_side + l ];
			
			if( !alldiff_concept( partial_sol ) )
			{
				cout << "Killer Sudoku - Error in square (" << i+1 << "," << j+1 << "): ";
				std::copy( partial_sol.begin(),
				           partial_sol.end(),
				           std::ostream_iterator<int>( cout, " " ) );
				cout << "\n";

				success = false;
			}
		}

	// Cages
	success = success && check_cage( solution, 0, 3, 0, 1 );
	success = success && check_cage( solution, 1, 15, 2, 3, 4 );
	success = success && check_cage( solution, 2, 22, 5, 13, 14, 22 );
	success = success && check_cage( solution, 3, 4, 6, 15 );
	success = success && check_cage( solution, 4, 16, 7, 16 );
	success = success && check_cage( solution, 5, 15, 8, 17, 26, 35 );
	success = success && check_cage( solution, 6, 25, 9, 10, 18, 19 );
	success = success && check_cage( solution, 7, 17, 11, 12 );
	success = success && check_cage( solution, 8, 9, 20, 21, 30 );
	success = success && check_cage( solution, 9, 8, 23, 32, 41 );
	success = success && check_cage( solution, 10, 20, 24, 25, 33 );
	success = success && check_cage( solution, 11, 6, 27, 36 );
	success = success && check_cage( solution, 12, 14, 28, 29 );
	success = success && check_cage( solution, 13, 17, 31, 40, 49 );
	success = success && check_cage( solution, 14, 17, 34, 42, 43 );
	success = success && check_cage( solution, 15, 13, 37, 38, 46 );
	success = success && check_cage( solution, 16, 20, 39, 48, 57 );
	success = success && check_cage( solution, 17, 12, 44, 53 );
	success = success && check_cage( solution, 18, 27, 45, 54, 63, 72 );
	success = success && check_cage( solution, 19, 6, 47, 55, 56 );
	success = success && check_cage( solution, 20, 20, 50, 59, 60 );
	success = success && check_cage( solution, 21, 6, 51, 52 );
	success = success && check_cage( solution, 22, 10, 58, 66, 67, 75 );
	success = success && check_cage( solution, 23, 14, 61, 62, 70, 71 );
	success = success && check_cage( solution, 24, 8, 64, 73 );
	success = success && check_cage( solution, 25, 16, 65, 74 );
	success = success && check_cage( solution, 26, 15, 68, 69 );
	success = success && check_cage( solution, 27, 13, 76, 77, 78 );
	success = success && check_cage( solution, 28, 17, 79, 80 );

	return success;
}

///////////////////////

int main( int argc, char **argv )
{
	bool parallel = false;
	int cores = -1;
	
	if( argc >= 2 )
			parallel = ( std::stoi( argv[1] ) != 0 );
	if( argc == 3 && parallel )
		cores = std::stoi( argv[2] );
	
  shared_ptr<Print> printer = make_shared<PrintSudoku>();

  BuilderKillerSudoku builder;
  Solver solver( builder );

  double error;
  vector<int> solution;

	Options options;
	options.print = printer;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );
	
  solver.solve( error, solution, 5s, options );		

	bool success = check_solution( solution );
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

