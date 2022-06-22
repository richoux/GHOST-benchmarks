#include <cmath>
#include <algorithm>

#include "linear-leq.hpp"

LinearLeq::LinearLeq( const vector<int>& variables, int rhs )
	: Constraint( variables ),
	  _rhs( rhs )
{ }

LinearLeq::LinearLeq( vector<int>&& variables, int rhs )
	: Constraint( std::move( variables ) ),
	  _rhs( rhs )
{ }

double LinearLeq::required_error( const vector<Variable*>& variables ) const
{
	double sum = 0.;
	
	for( auto& v : variables )
		sum += v->get_value();

	return std::max( 0., sum - _rhs );
}
