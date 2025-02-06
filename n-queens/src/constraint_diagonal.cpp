#include <algorithm>

#include "constraint_diagonal.hpp"

using namespace ghost;

Diagonal::Diagonal( const std::vector<int>& variables_index,
                    const std::vector<int>& positions )
	: Constraint( variables_index ),
    _positions( positions )
{ }

double Diagonal::required_error( const std::vector<Variable*>& variables ) const
{
	double sum = 0.;

  for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
	  if( variables[i]->get_value() == _positions[i] )
		  ++sum;
  
  return std::max( 0., sum - 1 );
}
