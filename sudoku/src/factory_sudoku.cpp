#include "factory_sudoku.hpp"

FactorySudoku::FactorySudoku( const std::vector<Variable>& variables, 
                              int instance_size,
                              bool hard_instance )
	: FactoryModel( variables ),
	  _instance_size( instance_size ),
	  _side_size( instance_size * instance_size ),
	  _hard_instance( hard_instance ),
	  _rows( vector< vector< Variable > >( _side_size ) ),
	  _columns( vector< vector< Variable > >( _side_size ) ),
	  _squares( vector< vector< Variable > >( _side_size ) )
{
  // Prepare row variables
  for( int r = 0; r < _side_size; ++r )
	  std::copy_n( variables.begin() + ( r * _side_size ),
	               _side_size,
	               std::back_inserter( _rows[r] ) );
  
  // Prepare column variables
  for( int c = 0; c < _side_size; ++c )
	  for( int line = 0; line < _side_size; ++line )
		  _columns[c].push_back( variables[ c + ( line * _side_size ) ] );
	  
  // Prepare square variables
  for( int s_r = 0; s_r < _instance_size; ++s_r )
	  for( int s_c = 0; s_c < _instance_size; ++s_c )
		  for( int line = 0; line < _instance_size; ++line )
			  std::copy_n( variables.begin() + ( ( s_r * _side_size * _instance_size )
			                                     + ( s_c * _instance_size )
			                                     + ( line * _side_size ) ),
			               _instance_size,
			               std::back_inserter( _squares[ ( s_r * _instance_size ) + s_c ] ) );
}

std::shared_ptr<Model> FactorySudoku::make_model()
{
	constraints.clear();
	
  for( int i = 0; i < _side_size; ++i )
  {
	  constraints.emplace_back( make_shared<AllDiff>( _rows[i] ) );
	  constraints.emplace_back( make_shared<AllDiff>( _columns[i] ) );
	  constraints.emplace_back( make_shared<AllDiff>( _squares[i] ) );
  }

  if( _hard_instance )
  {
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[5]}, 3 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[7]}, 1 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[8]}, 7 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[10]}, 1 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[11]}, 5 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[14]}, 9 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[17]}, 8 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[19]}, 6 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[27]}, 1 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[32]}, 7 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[38]}, 9 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[42]}, 2 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[48]}, 5 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[53]}, 4 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[61]}, 2 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[63]}, 5 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[66]}, 6 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[69]}, 3 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[70]}, 4 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[72]}, 3 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[73]}, 4 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector< Variable >{variables[75]}, 2 ) );
  }

  return make_shared<Model>( variables, constraints, objective );  
}
