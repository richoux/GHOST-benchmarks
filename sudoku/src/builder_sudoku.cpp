#include <ghost/global_constraints/all_different.hpp>
#include <ghost/global_constraints/fix_value.hpp>

#include "builder_sudoku.hpp"

BuilderSudoku::BuilderSudoku( int instance_size,
                              bool hard_instance )
	: ModelBuilder( true ),
	  _instance_size( instance_size ),
	  _side_size( instance_size * instance_size ),
	  _hard_instance( hard_instance ),
	  _rows( std::vector< std::vector<int> >( _side_size ) ),
	  _columns( std::vector< std::vector<int> >( _side_size ) ),
	  _squares( std::vector< std::vector<int> >( _side_size ) )
{
	// Prepare row variables
	for( int r = 0; r < _side_size; ++r )
  {
	  for( int e = r * _side_size ; e < ( r + 1 ) * _side_size ; ++e )
		  _rows[r].push_back( e );
  }
  
  // Prepare column variables
  for( int c = 0; c < _side_size; ++c )
  {
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
}

void BuilderSudoku::declare_variables()
{
	// Create variables
	int nb_vars = _side_size*_side_size;
	create_n_variables( nb_vars, 1, _side_size );

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
  for( int i = 0; i < _side_size; ++i )
  {
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::AllDifferent>( _rows[i] ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::AllDifferent>( _columns[i] ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::AllDifferent>( _squares[i] ) );
  }

  if( _hard_instance )
  {
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{7, 10, 27}, 1 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{42, 61, 75}, 2 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{5, 69, 72}, 3 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{53, 70, 73}, 4 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{11, 48, 63}, 5 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{19, 66}, 6 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{8, 32}, 7 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{17}, 8 ) );
	  constraints.emplace_back( std::make_shared<ghost::global_constraints::FixValue>( std::vector<int>{14, 38}, 9 ) );
  }
}
