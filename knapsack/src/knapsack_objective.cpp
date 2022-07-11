#include "knapsack_objective.hpp"

KSObjective::KSObjective( const std::vector<ghost::Variable>& variables, const std::vector<double>& values )
	: Maximize( variables, "Max profit" ),
	  _values( values )
{ }

double KSObjective::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double sum = 0.0;
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		sum += variables[i]->get_value() * _values[i];

	return sum;
}
