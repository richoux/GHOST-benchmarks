#include <cmath>

#include "fix_value.hpp"

FixValue::FixValue( const vector<int>& variables_index, int value )
	: Constraint( variables_index ),
	  _value( value ),
	  _current_diff( 0 )
{ }

double FixValue::required_error( const vector<Variable*>& variables ) const
{
	_current_diff = variables[0]->get_value() - _value;
	return std::abs( _current_diff );
}

double FixValue::optional_delta_error( const vector<Variable*>& variables,
                                       const vector<int>& variable_indexes,
                                       const vector<int>& candidate_values ) const
{
	int diff = _current_diff + ( candidate_values[0] - variables[ variable_indexes[0] ]->get_value() );
	
	return std::abs( diff ) - std::abs( _current_diff );
} 
