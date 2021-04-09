#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>

#include <ghost/solver.hpp>
#include <ghost/variable.hpp>

#if defined HAMMING
#include "linear-eq_hamming.hpp"
#else
#include "linear-eq_num.hpp"
#endif

#include "print_ms.hpp"

using namespace ghost;
using namespace std;

void check_solution( const vector<int>& solution, int constant )
{
	int nb_vars = static_cast<int>( solution.size() );
	int order = static_cast<int>( std::sqrt( nb_vars ) );
	int sum = 0;	
	vector<int> partial_sol( order );

	// Rows
	for( int i = 0; i < order; ++i )
	{
		std::copy( solution.begin() + ( i * order ),
		           solution.begin() + ( ( i + 1 ) * order ),
		           partial_sol.begin() );

		sum = std::accumulate( partial_sol.begin(), partial_sol.end(), 0 );
		if( sum != constant )
		{			
			cout << "Problem in row " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "=> the sum is " << sum << " rather than " << constant << "\n";
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
			cout << "Problem in column " << i+1 << ": ";
			std::copy( partial_sol.begin(),
			           partial_sol.end(),
			           std::ostream_iterator<int>( cout, " " ) );
			cout << "=> the sum is " << sum << " rather than " << constant << "\n";
		}
	}

	// Diagonal 0
	for( int i = 0; i < order; ++i )
		partial_sol[i] = solution[ i + ( i * order ) ];
		                           
	sum = std::accumulate( partial_sol.begin(), partial_sol.end(), 0 );
	if( sum != constant )
	{
		cout << "Problem in diagonal 0: ";
		std::copy( partial_sol.begin(),
		           partial_sol.end(),
		           std::ostream_iterator<int>( cout, " " ) );
		cout << "=> the sum is " << sum << " rather than " << constant << "\n";
	}

	// Diagonal 1
	for( int i = 0; i < order; ++i )
		partial_sol[i] = solution[ order - 1 - i + ( i * order ) ];
		                           
	sum = std::accumulate( partial_sol.begin(), partial_sol.end(), 0 );
	if( sum != constant )
	{
		cout << "Problem in diagonal 1: ";
		std::copy( partial_sol.begin(),
		           partial_sol.end(),
		           std::ostream_iterator<int>( cout, " " ) );
		cout << "=> the sum is " << sum << " rather than " << constant << "\n";
	}
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
	
	int nb_vars = order * order;
	int constant = order * ( nb_vars + 1 ) / 2;


	
  // Create variables, with domains starting from value 1
  vector<Variable> variables;
  for( int i = 0; i < nb_vars; ++i )
		variables.emplace_back( std::string("v") + std::to_string(i), 1, nb_vars );

  if( order != 8 )
	  for( int i = 0; i < nb_vars; ++i )
		  variables[i].set_value( i + 1 );
  else
  {
	  int i = 0;
	  variables[i++].set_value( 51 ); variables[i++].set_value( 63 ); variables[i++].set_value( 61 ); variables[i++].set_value( 44 ); variables[i++].set_value( 17 ); variables[i++].set_value( 37 ); variables[i++].set_value( 30 );  variables[i++].set_value( 6 ); 
	  variables[i++].set_value( 21 ); variables[i++].set_value( 62 ); variables[i++].set_value( 35 ); variables[i++].set_value( 28 ); variables[i++].set_value( 12 ); variables[i++].set_value( 45 ); variables[i++].set_value( 5 );  variables[i++].set_value( 60 ); 
	  variables[i++].set_value( 54 ); variables[i++].set_value( 13 ); variables[i++].set_value( 11 ); variables[i++].set_value( 36 ); variables[i++].set_value( 20 ); variables[i++].set_value( 29 ); variables[i++].set_value( 46 );  variables[i++].set_value( 10 ); 
	  variables[i++].set_value( 49 ); variables[i++].set_value( 1 ); variables[i++].set_value( 3 ); variables[i++].set_value( 40 ); variables[i++].set_value( 48 ); variables[i++].set_value( 64 ); variables[i++].set_value( 15 );  variables[i++].set_value( 43 ); 
	  variables[i++].set_value( 9 ); variables[i++].set_value( 47 ); variables[i++].set_value( 16 ); variables[i++].set_value( 2 ); variables[i++].set_value( 26 ); variables[i++].set_value( 31 ); variables[i++].set_value( 22 );  variables[i++].set_value( 34 ); 
	  variables[i++].set_value( 24 ); variables[i++].set_value( 42 ); variables[i++].set_value( 50 ); variables[i++].set_value( 7 ); variables[i++].set_value( 53 ); variables[i++].set_value( 27 ); variables[i++].set_value( 56 );  variables[i++].set_value( 58 ); 
	  variables[i++].set_value( 32 ); variables[i++].set_value( 25 ); variables[i++].set_value( 18 ); variables[i++].set_value( 39 ); variables[i++].set_value( 33 ); variables[i++].set_value( 14 ); variables[i++].set_value( 55 );  variables[i++].set_value( 38 ); 
	  variables[i++].set_value( 23 ); variables[i++].set_value( 52 ); variables[i++].set_value( 4 ); variables[i++].set_value( 59 ); variables[i++].set_value( 19 ); variables[i++].set_value( 8 ); variables[i++].set_value( 41 );  variables[i++].set_value( 57 ); 
  }
	  
  vector< vector<Variable> > rows( order );
  vector< vector<Variable> > columns( order );
  vector< vector<Variable> > diagonals( 2 );

  // Prepare row variables
  for( int row = 0; row < order; ++row )
	  std::copy_n( variables.begin() + ( row * order ),
	               order,
	               std::back_inserter( rows[ row ] ) );
  
  // Prepare column variables
  for( int col = 0; col < order; ++col )
	  for( int row = 0; row < order; ++row )
		  columns[ col ].push_back( variables[ col + ( row * order ) ] );
	  
  // Prepare square variables
  for( int row = 0; row < order; ++row )
  {
	  diagonals[ 0 ].push_back( variables[ row + ( row * order ) ] );
	  diagonals[ 1 ].push_back( variables[ order - 1 - row + ( row * order ) ] );
  }

  vector< LinearEq > constraint_rows;
  vector< LinearEq > constraint_columns;
  vector< LinearEq > constraint_diagonals;

  for( int i = 0; i < order; ++i )
  {
	  constraint_rows.emplace_back( rows[i], constant );
	  constraint_columns.emplace_back( columns[i], constant );
  }
  
  constraint_diagonals.emplace_back( diagonals[0], constant );
  constraint_diagonals.emplace_back( diagonals[1], constant );

  vector< variant<LinearEq> > constraints;

  std::move( constraint_rows.begin(),
             constraint_rows.end(),
             std::back_inserter( constraints ) );

  std::move( constraint_columns.begin(),
             constraint_columns.end(),
             std::back_inserter( constraints ) );

  std::move( constraint_diagonals.begin(),
             constraint_diagonals.end(),
             std::back_inserter( constraints ) );

  std::shared_ptr<Print> printer = std::make_shared<PrintMagicSquare>();
	Options options;
	options.print = printer;

	if( order == 8 )
		options.custom_starting_point = true;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );

  // true means it is a permutation problem
  Solver solver( variables, constraints, true );

  double error = 0.0;
  vector<int> solution( variables.size(), 0 );

	// 2min
	//solver.solve( error, solution, 120000000 );

	// 30s
	// solver.solve( error, solution, 30000000 );
	
  // 5s
  if( order != 8 )
	  solver.solve( error, solution, 5000000, options );
  else
	  solver.solve( error, solution, 5000000, options );

	// 0.5s
	//solver.solve( error, solution, 500000 );

	cout << "Error: " << error << "\n";
	check_solution( solution, constant );
	
  return EXIT_SUCCESS;
}

