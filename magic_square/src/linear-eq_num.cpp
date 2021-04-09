#include <cmath>
#include <algorithm>
#include <map>

#include "linear-eq_num.hpp"

#include <iostream>

LinearEq::LinearEq( const vector<Variable>& variables, int rhs )
	: Constraint( variables ),
	  _rhs( rhs ),
	  _current_diff( 0 )
{ }

double LinearEq::required_error( const vector< Variable >& variables ) const
{
	int sum = 0;
	for( const Variable& var : variables )
		sum += var.get_value();

	_current_diff = sum - _rhs;
	
	return std::abs( _current_diff );
}

double LinearEq::expert_delta_error( const vector<Variable>& variables,
                                     const vector<unsigned int>& variable_indexes,
                                     const vector<int>& candidate_values ) const
{
	int diff = _current_diff;

	for( int i = 0 ; i < static_cast<int>( variable_indexes.size() ); ++i )
		diff += ( candidate_values[ i ] - variables[ variable_indexes[i] ].get_value() );
	
	return std::abs( diff ) - std::abs( _current_diff );
} 

void LinearEq::update_constraint( const vector<Variable>& variables, unsigned int variable_index, int new_value ) 
{
	_current_diff += ( new_value - variables[ variable_index ].get_value() );
}