#include "knapsack_objective.hpp"

KSObjective::KSObjective( const std::vector<ghost::Variable>& variables, std::shared_ptr<ghost::AuxiliaryData> data )
	: Maximize( variables, "Max profit" ),
	  _values( std::dynamic_pointer_cast<KSCoefficients>(data)->values )
{ }

double KSObjective::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double sum = 0.0;
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		sum += variables[i]->get_value() * _values[i];

	return sum;
}
