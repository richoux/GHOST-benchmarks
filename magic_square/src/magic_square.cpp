#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ghost/solver.hpp>

#include "builder_ms.hpp"
#include "print_ms.hpp"

using namespace ghost;
using namespace std;
using namespace std::literals::chrono_literals;

bool check_solution( const vector<int>& solution, int constant )
{
	int nb_vars = static_cast<int>( solution.size() );
	int order = static_cast<int>( std::sqrt( nb_vars ) );
	int sum = 0;	
	vector<int> partial_sol( order );
	bool success = true;
	
	// Rows
	for( int i = 0; i < order; ++i )
	{
		std::copy( solution.begin() + ( i * order ),
		           solution.begin() + ( ( i + 1 ) * order ),
		           partial_sol.begin() );

		sum = std::accumulate( partial_sol.begin(), partial_sol.end(), 0 );
		if( sum != constant )
		{			
			cout << "Magic Square - Error in row " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "=> the sum is " << sum << " rather than " << constant << "\n";
			success =  false;
		}
	}

	// Columns
	for( int i = 0; i < order; ++i )
	{
		for( int j = 0; j < order; ++j )
			partial_sol[j] = solution[ j * order + i ];

		sum = std::accumulate( partial_sol.begin(), partial_sol.end(), 0 );
		if( sum != constant )
		{
			cout << "Magic Square - Error in column " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "=> the sum is " << sum << " rather than " << constant << "\n";
			success =  false;
		}
	}

	// Diagonal 0
	for( int i = 0; i < order; ++i )
		partial_sol[i] = solution[ i + ( i * order ) ];
		                           
	sum = std::accumulate( partial_sol.begin(), partial_sol.end(), 0 );
	if( sum != constant )
	{
		cout << "Magic Square - Error in diagonal 0: ";
		std::copy( partial_sol.begin(),
		           partial_sol.end(),
		           std::ostream_iterator<int>( cout, " " ) );
		cout << "=> the sum is " << sum << " rather than " << constant << "\n";
		success =  false;
	}

	// Diagonal 1
	for( int i = 0; i < order; ++i )
		partial_sol[i] = solution[ order - 1 - i + ( i * order ) ];
		                           
	sum = std::accumulate( partial_sol.begin(), partial_sol.end(), 0 );
	if( sum != constant )
	{
		cout << "Magic Square - Error in diagonal 1: ";
		std::copy( partial_sol.begin(),
		           partial_sol.end(),
		           std::ostream_iterator<int>( cout, " " ) );
		cout << "=> the sum is " << sum << " rather than " << constant << "\n";
		success =  false;
	}

	return success;
}

///////////////////////

int main( int argc, char **argv )
{
	int order;
	bool parallel = false;
	int cores = -1;

	if( argc == 1 )
	{
		std::cout << "Usage: " << argv[0] << " N [parallel=0/1] [number_threads]\n";
		return EXIT_FAILURE;
	}
	else
	{
		order = std::stoi( argv[1] );
		if( argc >= 3 )
			parallel = ( std::stoi( argv[2] ) != 0 );
		if( argc == 4 && parallel )
			cores = std::stoi( argv[3] );
	}
		
  std::shared_ptr<Print> printer = std::make_shared<PrintMagicSquare>();
	Options options;
	options.print = printer;

	// Parameters tuning from Diaz et al.
	options.percent_chance_escape_plateau = 6;
	options.tabu_time_selected = 0;
	options.tabu_time_local_min = 5;
	options.reset_threshold = order;
	options.restart_threshold = 0;
	options.number_variables_to_reset = std::max( 1, order / 10 );
	
	if( order == 8 )
		options.custom_starting_point = true;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );

	BuilderMagicSquare builder( order );
	
  // true means it is a permutation problem
  Solver solver( builder );

  double error;
  vector<int> solution;

  solver.solve( error, solution, 1s, options );

	bool success = check_solution( solution, order * ( order * order + 1 ) / 2 );
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

