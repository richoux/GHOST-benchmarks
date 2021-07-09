#include "knapsack_capacity.hpp"

KSCapacity::KSCapacity( const std::vector<ghost::Variable>& variables, int capacity, std::shared_ptr<ghost::AuxiliaryData> data )
	: Constraint( variables ),
	  _capacity( capacity ),
	  _weights( std::dynamic_pointer_cast<KSCoefficients>(data)->weights )
{ }

double KSCapacity::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	// Common stuff
	double sum = 0.0;
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		sum += variables[i]->get_value() * _weights[i];

	/*
	// COP model
	if( sum <= _capacity )
		return 0.0;
	else
		return 1.0;
	/*/
	// EFOP model
	return std::max( 0.0, sum - _capacity );
	//*/
}
