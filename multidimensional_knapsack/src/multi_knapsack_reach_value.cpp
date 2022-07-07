#include "multi_knapsack_reach_value.hpp"

MultiKSReachValue::MultiKSReachValue( const std::vector<ghost::Variable>& variables, const std::vector<int>& values, int optimal )
	: Constraint( variables ),
	  _values( values ),
	  _optimal( optimal )
{ }

double MultiKSReachValue::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	double sum = 0.0;
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		sum += variables[i]->get_value() * _values[i];

	return std::max( 0.0, _optimal - sum );
}