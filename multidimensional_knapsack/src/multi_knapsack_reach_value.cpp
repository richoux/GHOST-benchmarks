#include "multi_knapsack_reach_value.hpp"

MultiKSReachValue::MultiKSReachValue( const std::vector<ghost::Variable>& variables, const std::vector<int>& values, int optimal )
	: Constraint( variables ),
	  _values( values ),
	  _optimal( optimal )
{ }

double MultiKSReachValue::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	_cache_sum_values = 0.0;
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		_cache_sum_values += variables[i]->get_value() * _values[i];

	return std::max( 0.0, _optimal - _cache_sum_values );
}

double MultiKSReachValue::optional_delta_error( const std::vector<ghost::Variable*>& variables, const std::vector<int>& indexes, const std::vector<int>& candidate_values ) const
{
	double new_sum = _cache_sum_values;

	for( int i = 0 ; i < static_cast<int>( indexes.size() ) ; ++i )
		new_sum += ( ( candidate_values[ i ] - variables[ indexes[i] ]->get_value() ) * _values[ indexes[i] ] );
	
	return std::max( 0.0, _optimal - new_sum ) - get_current_error();
}

void MultiKSReachValue::conditional_update_data_structures( const std::vector<ghost::Variable*>& variables, int variable_index, int new_value )
{
	_cache_sum_values += ( ( new_value - variables[ variable_index ]->get_value() ) * _values[ variable_index ] );
}
