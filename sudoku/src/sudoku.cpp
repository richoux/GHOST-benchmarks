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
	int size_side_small_square;
	bool hard_instance = false;

	bool parallel = false;
	int cores = -1;
	
	if( argc == 1 )
		size_side_small_square = 3;
	else
	{
		size_side_small_square = std::stoi( argv[1] );
		if( argc >= 3 )
			parallel = ( std::stoi( argv[2] ) != 0 );
		if( argc == 4 && parallel )
			cores = std::stoi( argv[3] );
	}

	if( size_side_small_square < 3 )
	{
		size_side_small_square = 3;
		hard_instance = true;
	}
	
	int size_side = size_side_small_square * size_side_small_square;
	int nb_vars = size_side * size_side;
  
  // Create variables
  vector< Variable > variables;
  for( int i = 0; i < nb_vars; ++i )
		variables.emplace_back( std::string("v") + std::to_string(i), 1, size_side );

  if( hard_instance )
  {
	  /*
	    0 0 0  0 0 3  0 1 7 
	    0 1 5  0 0 9  0 0 8 
	    0 6 0  0 0 0  0 0 0 

	    1 0 0  0 0 7  0 0 0 
	    0 0 9  0 0 0  2 0 0 
	    0 0 0  5 0 0  0 0 4 

	    0 0 0  0 0 0  0 2 0 
	    5 0 0  6 0 0  3 4 0 
	    3 4 0  2 0 0  0 0 0
	    *
	    * Solution:
	    2 9 4  8 6 3  5 1 7 
	    7 1 5  4 2 9  6 3 8 
	    8 6 3  7 5 1  4 9 2 

	    1 5 2  9 4 7  8 6 3 
	    4 7 9  3 8 6  2 5 1 
	    6 3 8  5 1 2  9 7 4 

	    9 8 6  1 3 4  7 2 5 
	    5 2 1  6 7 8  3 4 9 
	    3 4 7  2 9 5  1 8 6
	   */
	  int i = 0;
	  variables[i++].set_value(2); variables[i++].set_value(4); variables[i++].set_value(5); variables[i++].set_value(6); variables[i++].set_value(8); variables[i++].set_value(3); variables[i++].set_value(9); variables[i++].set_value(1); variables[i++].set_value(7);
	  variables[i++].set_value(2); variables[i++].set_value(1); variables[i++].set_value(5); variables[i++].set_value(3); variables[i++].set_value(4); variables[i++].set_value(9); variables[i++].set_value(6); variables[i++].set_value(7); variables[i++].set_value(8);
	  variables[i++].set_value(1); variables[i++].set_value(6); variables[i++].set_value(2); variables[i++].set_value(3); variables[i++].set_value(4); variables[i++].set_value(5); variables[i++].set_value(7); variables[i++].set_value(8); variables[i++].set_value(9);
	  variables[i++].set_value(1); variables[i++].set_value(2); variables[i++].set_value(3); variables[i++].set_value(4); variables[i++].set_value(5); variables[i++].set_value(7); variables[i++].set_value(6); variables[i++].set_value(8); variables[i++].set_value(9);
	  variables[i++].set_value(1); variables[i++].set_value(3); variables[i++].set_value(9); variables[i++].set_value(4); variables[i++].set_value(5); variables[i++].set_value(6); variables[i++].set_value(2); variables[i++].set_value(7); variables[i++].set_value(8);
	  variables[i++].set_value(1); variables[i++].set_value(2); variables[i++].set_value(3); variables[i++].set_value(5); variables[i++].set_value(6); variables[i++].set_value(7); variables[i++].set_value(8); variables[i++].set_value(9); variables[i++].set_value(4);
	  variables[i++].set_value(1); variables[i++].set_value(3); variables[i++].set_value(4); variables[i++].set_value(5); variables[i++].set_value(6); variables[i++].set_value(7); variables[i++].set_value(8); variables[i++].set_value(2); variables[i++].set_value(9);
	  variables[i++].set_value(5); variables[i++].set_value(1); variables[i++].set_value(2); variables[i++].set_value(6); variables[i++].set_value(7); variables[i++].set_value(8); variables[i++].set_value(3); variables[i++].set_value(4); variables[i++].set_value(9);
	  variables[i++].set_value(3); variables[i++].set_value(4); variables[i++].set_value(1); variables[i++].set_value(2); variables[i++].set_value(5); variables[i++].set_value(6); variables[i++].set_value(7); variables[i++].set_value(8); variables[i++].set_value(9);
  }
  else
	  for( int i = 0; i < nb_vars; ++i )
		  variables[i].set_value( ( i % size_side ) + 1 );
	  
  shared_ptr<Print> printer = make_shared<PrintSudoku>();

  FactorySudoku factory( variables, size_side_small_square, hard_instance );
  Solver solver( factory, true );

  double error = 0.0;
	vector<int> solution( variables.size(), 0 );

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

