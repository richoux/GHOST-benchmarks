#include "factory_ms.hpp"

FactoryMagicSquare::FactoryMagicSquare( const std::vector<Variable>& variables, 
                                        int instance_size )
	: FactoryModel( variables ),
	  _instance_size( instance_size ),
	  _nb_vars( instance_size * instance_size),
	  _constant( instance_size * ( _nb_vars + 1 ) / 2 ),
	  _rows( vector< vector< Variable > >( instance_size ) ),
	  _columns( vector< vector< Variable > >( instance_size ) ),
	  _diagonals( vector< vector< Variable > >( 2 ) )	  
{
	// Prepare row variables
  for( int row = 0; row < _instance_size; ++row )
	  std::copy_n( variables.begin() + ( row * _instance_size ),
	               _instance_size,
	               std::back_inserter( _rows[ row ] ) );
  
  // Prepare column variables
  for( int col = 0; col < _instance_size; ++col )
	  for( int row = 0; row < _instance_size; ++row )
		  _columns[ col ].push_back( variables[ col + ( row * _instance_size ) ] );
	  
  // Prepare square variables
  for( int row = 0; row < _instance_size; ++row )
  {
	  _diagonals[ 0 ].push_back( variables[ row + ( row * _instance_size ) ] );
	  _diagonals[ 1 ].push_back( variables[ _instance_size - 1 - row + ( row * _instance_size ) ] );
  }
}
	
std::shared_ptr<Model> FactoryMagicSquare::make_model()
{
	constraints.clear();
	
	for( int i = 0; i < _instance_size; ++i )
  {
	  constraints.emplace_back( make_shared<LinearEq>( _rows[i], _constant ) );
	  constraints.emplace_back( make_shared<LinearEq>( _columns[i], _constant ) );
  }
  
	constraints.emplace_back( make_shared<LinearEq>( _diagonals[0], _constant ) );
	constraints.emplace_back( make_shared<LinearEq>( _diagonals[1], _constant ) );

  return make_shared<Model>( variables, constraints, objective );  
}
