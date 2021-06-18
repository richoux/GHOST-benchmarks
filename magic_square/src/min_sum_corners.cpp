#include <cmath>
#include "min_sum_corners.hpp"

MinCorners::MinCorners( const vector<Variable>& variables )
	: Objective( variables, "MinSumCorners" ),
	  _number_variables( static_cast<int>( variables.size() ) ),
	  _instance_size( static_cast<int>( std::sqrt( variables.size() ) ) )
{ }

double MinCorners::required_cost( const vector<Variable*>& variables ) const
{
	return variables[0]->get_value() + variables[_instance_size - 1]->get_value() + variables[_number_variables - 1]->get_value() + variables[_number_variables - _instance_size ]->get_value();
}
