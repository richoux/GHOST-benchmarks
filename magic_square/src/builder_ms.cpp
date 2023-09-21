#include <ghost/global_constraints/linear_equation_eq.hpp>
#if defined COMPLETESEARCH
#include <ghost/global_constraints/all_different.hpp>
#endif

#include "builder_ms.hpp"

BuilderMagicSquare::BuilderMagicSquare( int instance_size )
#if defined COMPLETESEARCH
	: ghost::ModelBuilder(),
#else
	: ghost::ModelBuilder( true ),
#endif
	  _instance_size( instance_size ),
	  _nb_vars( instance_size * instance_size),
	  _constant( instance_size * ( _nb_vars + 1 ) / 2 ),
	  _rows( std::vector< std::vector<int> >( instance_size ) ),
	  _columns( std::vector< std::vector<int> >( instance_size ) ),
	  _diagonals( std::vector< std::vector<int> >( 2 ) )	  
{
	// Prepare row variables
  for( int row = 0; row < _instance_size; ++row )
	  for( int i = 0 ; i < _instance_size ; ++i )
		  _rows[row].push_back( i + ( row * _instance_size ) );
  
  // Prepare column variables
  for( int col = 0; col < _instance_size; ++col )
	  for( int row = 0; row < _instance_size; ++row )
		  _columns[ col ].push_back( col + ( row * _instance_size ) );
	  
  // Prepare square variables
  for( int row = 0; row < _instance_size; ++row )
  {
	  _diagonals[ 0 ].push_back( row + ( row * _instance_size ) );
	  _diagonals[ 1 ].push_back( _instance_size - 1 - row + ( row * _instance_size ) );
  }
}

void BuilderMagicSquare::declare_variables()
{
  // Create variables, with domains starting from value 1
	create_n_variables( _nb_vars, 1, _nb_vars );
	
#if not defined COMPLETESEARCH
	if( _instance_size != 8 )
		for( int i = 0; i < _nb_vars; ++i )
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
#endif
}

void BuilderMagicSquare::declare_constraints()
{
#if defined COMPLETESEARCH	
	constraints.emplace_back( std::make_shared<ghost::global_constraints::AllDifferent>( variables ) );
#endif

	for( int i = 0; i < _instance_size; ++i )
  {
	  // constraints.emplace_back( make_shared<LinearEq>( _rows[i], _constant ) );
	  // constraints.emplace_back( make_shared<LinearEq>( _columns[i], _constant ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationEq>( _rows[i], _constant ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationEq>( _columns[i], _constant ) );
  }
  
	// constraints.emplace_back( make_shared<LinearEq>( _diagonals[0], _constant ) );
	// constraints.emplace_back( make_shared<LinearEq>( _diagonals[1], _constant ) );
	constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationEq>( _diagonals[0], _constant ) );
	constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationEq>( _diagonals[1], _constant ) );
}

#if defined MINMS
void BuilderMagicSquare::declare_objective()
{
	objective = std::make_shared<MinCorners>( variables );
}
#endif
