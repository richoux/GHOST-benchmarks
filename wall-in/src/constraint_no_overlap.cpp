#include "constraint_no_overlap.hpp"
#include "utils.hpp"

NoOverlap::NoOverlap( const std::vector<Variable>& variables, const Grid& grid )
	: Constraint(variables),
	  _grid( &grid )
{ }

double NoOverlap::required_error( const std::vector<Variable>& variables ) const
{
	double count = 0.0;
	
	for( int index = 0 ; index < static_cast<int>( variables.size() ) ; ++index )
		if( _grid->what_is_at( index ) > 1 )
			++count;

	return count;
}

// double NoOverlap::expert_delta_error( const std::vector<Variable>& variables,
//                                       const std::vector<unsigned int>& variable_indexes,
//                                       const std::vector<int>& candidate_values ) const
// {

// }
