#include <algorithm>

#include "constraint_max_streak.hpp"

using namespace ghost;

MaxStreak::MaxStreak( const std::vector<int>& variables_index )
	: Constraint( variables_index )
{ }

double MaxStreak::required_error( const std::vector<Variable*>& variables ) const
{
	double maxstreak = 0.;
	
	std::vector<int> weeks( variables.size() );
	std::transform( variables.begin(),
									variables.end(),
									weeks.begin(),
									[&](auto& var){return var->get_value();} );
	std::sort( weeks.begin(), weeks.end() );
	
	for( size_t i = 0 ; i < weeks.size() - 1 ; ++i )
		if( weeks[i] == weeks[i+1] - 1 )
			++maxstreak;
		else
			maxstreak = 0.;
	
	return std::max( 0.0, maxstreak - 3 );
}
