#include <algorithm>

#include "constraint_merge.hpp"
#include "actionMap.hpp"

Merge::Merge( const std::vector< int >& variables_index )
	: Constraint(variables_index)
{ }

double Merge::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	int count = 0;
	
	for( int i = 1 ; i < variables.size() ; ++i )
		if( variables[0]->get_value() > variables[i]->get_value() )
			++count;

	return std::max(0.0, 2.0 - count);
}
