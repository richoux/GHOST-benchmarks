#include "builder_sudoku.hpp"

BuilderSudoku::BuilderSudoku( int instance_size,
                              bool hard_instance )
	: ModelBuilder(),
	  _instance_size( instance_size ),
	  _side_size( instance_size * instance_size ),
	  _hard_instance( hard_instance ),
	  _rows( vector< vector<int> >( _side_size ) ),
	  _columns( vector< vector<int> >( _side_size ) ),
	  _squares( vector< vector<int> >( _side_size ) )
{ }

void BuilderSudoku::declare_variables()
{
	// Create variables
	int nb_vars = _side_size*_side_size;
	for( int i = 0; i < nb_vars ; ++i )
		//variables.emplace_back( std::string("v") + std::to_string(i), 1, _side_size );
		variables.emplace_back( 1, _side_size );

	if( _hard_instance )
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
			variables[i].set_value( ( i % _side_size ) + 1 );
}

void BuilderSudoku::declare_constraints()
{
  // Prepare row variables
  for( int r = 0; r < _side_size; ++r )
  {
	  _rows[r].clear();
	  for( int e = r * _side_size ; e < ( r + 1 ) * _side_size ; ++e )
		  _rows[r].push_back( e );
  }
  
  // Prepare column variables
  for( int c = 0; c < _side_size; ++c )
  {
	  _columns[c].clear();
	  for( int e = 0; e < _side_size; ++e )
		  _columns[c].push_back( c + ( e * _side_size ) );
  }
  
  // Prepare square variables
  for( int s_r = 0; s_r < _instance_size; ++s_r )
	  for( int s_c = 0; s_c < _instance_size; ++s_c )
	  {
		  _squares[ ( s_r * _instance_size ) + s_c ].clear();
		  for( int c = 0; c < _instance_size; ++c )
			  for( int r = 0; r < _instance_size; ++r )
			  {
				  _squares[ ( s_r * _instance_size ) + s_c ].push_back( r + ( s_r * _side_size * _instance_size )
				                                                        + ( s_c * _instance_size )
				                                                        + ( c * _side_size ) );
			  }
	  }
	
  for( int i = 0; i < _side_size; ++i )
  {
	  constraints.emplace_back( make_shared<AllDiff>( _rows[i] ) );
	  constraints.emplace_back( make_shared<AllDiff>( _columns[i] ) );
	  constraints.emplace_back( make_shared<AllDiff>( _squares[i] ) );
  }

  if( _hard_instance )
  {
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{5}, 3 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{7}, 1 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{8}, 7 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{10}, 1 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{11}, 5 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{14}, 9 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{17}, 8 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{19}, 6 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{27}, 1 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{32}, 7 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{38}, 9 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{42}, 2 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{48}, 5 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{53}, 4 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{61}, 2 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{63}, 5 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{66}, 6 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{69}, 3 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{70}, 4 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{72}, 3 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{73}, 4 ) );
	  constraints.emplace_back( make_shared<FixValue>( vector<int>{75}, 2 ) );
  }
}
