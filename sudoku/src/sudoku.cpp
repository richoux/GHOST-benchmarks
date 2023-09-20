#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

#include <ghost/solver.hpp>

#include "builder_sudoku.hpp"
#include "print_sudoku.hpp"

using namespace std::literals::chrono_literals;

bool alldiff_concept( const std::vector<int>& var )
{
	// We assume our k variables can take values in [1, k]
	std::vector<bool> bitvec( var.size(), false );

	// Returns false if and only if we have two variables sharing the same value, 
	for( int i = 0 ; i < static_cast<int>( var.size() ) ; ++i )
		if( !bitvec[ var[i]-1 ] )
			bitvec[ var[i]-1 ] = true;
		else
			return false;
	
	return true;	
}

void check_hard_instance( const std::vector<int>& solution, int index, int expected_value )
{
	std::cout << "Sudoku - Error with the hard instance: var[" << index << "]=" << solution[index] << " but should be assigned at " << expected_value << ".\n";
}

bool check_solution( const std::vector<int>& solution, bool hard_instance )
{
	int nb_vars = solution.size();
	int size_side = static_cast<int>( std::sqrt( nb_vars ) );
	int size_side_small_square = static_cast<int>( std::sqrt( size_side ) );

	std::vector<int> partial_sol( size_side );
	bool success = true;
	
	// Rows
	for( int i = 0; i < size_side; ++i )
	{
		std::copy( solution.begin() + ( i * size_side ),
		           solution.begin() + ( ( i + 1 ) * size_side ),
		           partial_sol.begin() );

		if( !alldiff_concept( partial_sol ) )
		{
			std::cout << "Sudoku - Error in row " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( std::cout, " " ) );
			std::cout << "\n";
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
			std::cout << "Sudoku - Error in column " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( std::cout, " " ) );
			std::cout << "\n";
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
				std::cout << "Sudoku - Error in square (" << i+1 << "," << j+1 << "): ";
				std::copy( partial_sol.begin(),
				           partial_sol.end(),
				           std::ostream_iterator<int>( std::cout, " " ) );
				std::cout << "\n";
				success = false;
			}
		}

	if( hard_instance )
	{
		for( auto index : std::vector<int>{7, 10, 27} )
			if( solution[index] != 1 )
			{
				check_hard_instance( solution, index, 1 );
				success = false;
			}

		for( auto index : std::vector<int>{42, 61, 75} )
			if( solution[index] != 2 )
			{
				check_hard_instance( solution, index, 2 );
				success = false;
			}

		for( auto index : std::vector<int>{5, 69, 72} )
			if( solution[index] != 3 )
			{
				check_hard_instance( solution, index, 3 );
				success = false;
			}

		for( auto index : std::vector<int>{53, 70, 73} )
			if( solution[index] != 4 )
			{
				check_hard_instance( solution, index, 4 );
				success = false;
			}

		for( auto index : std::vector<int>{11, 48, 63} )
			if( solution[index] != 5 )
			{
				check_hard_instance( solution, index, 5 );
				success = false;
			}

		for( auto index : std::vector<int>{19, 66} )
			if( solution[index] != 6 )
			{
				check_hard_instance( solution, index, 6 );
				success = false;
			}

		for( auto index : std::vector<int>{8, 32} )
			if( solution[index] != 7 )
			{
				check_hard_instance( solution, index, 7 );
				success = false;
			}

		if( solution[17] != 8 )
		{
			check_hard_instance( solution, 17, 8 );
			success = false;
		}

		for( auto index : std::vector<int>{14, 38} )
			if( solution[index] != 9 )
			{
				check_hard_instance( solution, index, 9 );
				success = false;
			}
	}

	return success;
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
	
	std::shared_ptr<ghost::Print> printer = std::make_shared<PrintSudoku>();

  BuilderSudoku builder( instance_size, hard_instance );
  ghost::Solver solver( builder );

  double error;
  std::vector<int> solution;

  ghost::Options options;
	options.print = printer;
	if( hard_instance )
		options.custom_starting_point = true;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );
	
  solver.fast_search( error, solution, 5s, options );		

  bool success = check_solution( solution, hard_instance );
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

