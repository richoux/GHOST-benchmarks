#include <cmath>
#include <algorithm>

#include "linear-geq.hpp"

LinearGeq::LinearGeq( const vector<int>& variables, int rhs )
	: Constraint( variables ),
	  _rhs( rhs )
{ }

LinearGeq::LinearGeq( vector<int>&& variables, int rhs )
	: Constraint( std::move( variables ) ),
	  _rhs( rhs )
{ }

double LinearGeq::required_error( const vector<Variable*>& variables ) const
{
	double sum = 0.;
	
	for( auto& v : variables )
		sum += v->get_value();

	return std::max( 0., _rhs - sum );
}
