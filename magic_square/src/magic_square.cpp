#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>

#include <ghost/solver.hpp>
#include <ghost/variable.hpp>

#include "linear-eq.hpp"

using namespace ghost;
using namespace std;

void print_solution( const vector<int>& solution )
{
	int nb_vars = (int)solution.size();
	int order = static_cast<int>( std::sqrt( nb_vars ) );
	
	cout << "Solution:";
	
	for( int i = 0; i < nb_vars; ++i )
	{
		if( i%order == 0 )
			cout << "\n";
		
		cout << solution[i] << " ";
	}

	cout << "\n";
}

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
	if( argc == 1 )
		order = 3;
	else
		order = std::stoi( argv[1] );

	int nb_vars = order * order;
	int constant = order * ( nb_vars + 1 ) / 2;
	
  // Create variables, with domains starting from value 1
  vector<Variable> variables;
  for( int i = 0; i < nb_vars; ++i )
		variables.emplace_back( std::string("v") + std::to_string(i), 1, nb_vars );

  for( int i = 0; i < nb_vars; ++i )
	  variables[i].set_value( i + 1 );
	  
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

  // cout << "Constraint size: " << constraints.size() << "\n"
  //      << "Magic Square constant: " << constant << "\n";
  
  // true means it is a permutation problem
  Solver solver( variables, constraints, true );

  double error = 0.0;
	vector<int> solution( variables.size(), 0 );

	// 2min
	//solver.solve( error, solution, 120000000 );

	// 30s
	// solver.solve( error, solution, 30000000 );
	
  // 5s
	solver.solve( error, solution, 5000000 );

	// 0.5s
	//solver.solve( error, solution, 500000 );

	cout << "Error: " << error << "\n";
	print_solution( solution );
	check_solution( solution, constant );
	
  return EXIT_SUCCESS;
}

