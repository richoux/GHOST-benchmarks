#include <numeric>

#include "builder_nqueens.hpp"
#include "constraint_diagonal.hpp"

BuilderNQueens::BuilderNQueens( int n )
	: ModelBuilder( true ),
	  _n( n )
{ }

void BuilderNQueens::declare_variables()
{
	create_n_variables( _n, 0, _n );
	for( int i = 0; i < _n; ++i )
		variables[i].set_value( i );
}

void BuilderNQueens::declare_constraints()
{
	std::vector<int> variables_index{0};
		
  for( int c = 1; c <= _n-1; ++c )
  {
	  variables_index.push_back( c );

	  std::vector<int> diag_left_top( variables_index.size() );
	  std::iota( diag_left_top.rbegin(), diag_left_top.rend(), 0 );
	  constraints.emplace_back( std::make_shared<Diagonal>( variables_index, diag_left_top ) );
	  
	  std::vector<int> diag_left_bottom( variables_index.size() );
	  std::iota( diag_left_bottom.begin(), diag_left_bottom.end(), _n - (c+1) );
	  constraints.emplace_back( std::make_shared<Diagonal>( variables_index, diag_left_bottom ) );
  }

  for( int c = 1; c <= _n-2; ++c )
  {
	  variables_index.erase( variables_index.begin() );
	  std::vector<int> diag_left_top( variables_index.size() );
	  std::iota( diag_left_top.rbegin(), diag_left_top.rend(), c );
	  constraints.emplace_back( std::make_shared<Diagonal>( variables_index, diag_left_top ) );

	  std::vector<int> diag_left_bottom( variables_index.size() );
	  std::iota( diag_left_bottom.begin(), diag_left_bottom.end(), 0 );
	  constraints.emplace_back( std::make_shared<Diagonal>( variables_index, diag_left_bottom ) );
  }
}
