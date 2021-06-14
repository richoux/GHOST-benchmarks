#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>

#include <ghost/solver.hpp>

#include "factory_sudoku.hpp"
#include "print_sudoku.hpp"

using namespace ghost;
using namespace std;

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

void check_solution( const vector<int>& solution )
{
	int nb_vars = solution.size();
	int size_side = static_cast<int>( std::sqrt( nb_vars ) );
	int size_side_small_square = static_cast<int>( std::sqrt( size_side ) );

	vector<int> partial_sol( size_side );

	// Rows
	for( int i = 0; i < size_side; ++i )
	{
		std::copy( solution.begin() + ( i * size_side ),
		           solution.begin() + ( ( i + 1 ) * size_side ),
		           partial_sol.begin() );

		if( !alldiff_concept( partial_sol ) )
		{
			cout << "Problem in row " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "\n";
		}
	}

	// Columns
	for( int i = 0; i < size_side; ++i )
	{
		for( int j = 0; j < size_side; ++j )
			partial_sol[j] = solution[ j * size_side + i ];

		if( !alldiff_concept( partial_sol ) )
		{
			cout << "Problem in column " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "\n";
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
				cout << "Problem in square (" << i+1 << "," << j+1 << "): ";
				std::copy( partial_sol.begin(),
				           partial_sol.end(),
				           std::ostream_iterator<int>( cout, " " ) );
				cout << "\n";
			}
		}
}

///////////////////////

int main( int argc, char **argv )
{
	int instance_size;
	bool hard_instance = false;

	bool parallel = false;
	int cores = -1;
	
	if( argc == 1 )
		instance_size = 3;
	else
	{
		instance_size = std::stoi( argv[1] );
		if( argc >= 3 )
			parallel = ( std::stoi( argv[2] ) != 0 );
		if( argc == 4 && parallel )
			cores = std::stoi( argv[3] );
	}

	if( instance_size < 3 )
	{
		instance_size = 3;
		hard_instance = true;
	}
	
  shared_ptr<Print> printer = make_shared<PrintSudoku>();

  FactorySudoku factory( instance_size, hard_instance );
  Solver solver( factory, true );

  double error;
  vector<int> solution;

	Options options;
	options.print = printer;
	if( hard_instance )
		options.custom_starting_point = true;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );
	
	// 10min
	// solver.solve( error, solution, 600000000, options );
	
	// 2min
	// solver.solve( error, solution, 120000000, options );

	// 30s
	// solver.solve( error, solution, 30000000, options );
	
  // 5s
  solver.solve( error, solution, 5000000, options );		

	// 0.5s
	//solver.solve( error, solution, 500000, options );

	cout << "Error: " << error << "\n";
	//print_solution( solution );
	check_solution( solution );
	
  return EXIT_SUCCESS;
}

