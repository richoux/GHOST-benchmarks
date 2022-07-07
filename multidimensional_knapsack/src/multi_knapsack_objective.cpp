#include "multi_knapsack_objective.hpp"

MultiKSObjective::MultiKSObjective( const std::vector<ghost::Variable>& variables, const std::vector<int>& values )
	: Maximize( variables, "Max profit" ),
	  _values( values )
{ }

double MultiKSObjective::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double sum = 0.0;
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		sum += variables[i]->get_value() * _values[i];

	return sum;
}
