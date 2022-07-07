#include "multi_knapsack_capacity.hpp"

MultiKSCapacity::MultiKSCapacity( const std::vector<ghost::Variable>& variables, const std::vector<int>& coefficients, int capacity )
	: Constraint( variables ),
	  _coefficients( coefficients ),
	  _capacity( capacity )
{ }

double MultiKSCapacity::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	double sum = 0.0;
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		sum += variables[i]->get_value() * _coefficients[i];

	return std::max( 0.0, sum - _capacity );
}
