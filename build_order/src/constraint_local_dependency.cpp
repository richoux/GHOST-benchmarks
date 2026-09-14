#include <algorithm>

#include "constraint_local_dependency.hpp"
#include "actionMap.hpp"

Dependency::Dependency( const std::vector< int >& variables_index )
	: Constraint(variables_index)
{ }

// The first variable must be scheduled after at least one of its repeated dependencies, i.e., the other variables.
double Dependency::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	for( int i = 1 ; i < variables.size() ; ++i )
		if( variables[0]->get_value() > variables[i]->get_value() )
			return 0.0;
    
	return 1.0;
}
