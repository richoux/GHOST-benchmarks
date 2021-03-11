#include <cmath>
#include <algorithm>
#include <map>

#include "linear-eq_hamming.hpp"


LinearEq::LinearEq( const vector<Variable>& variables, int rhs )
	: Constraint( variables ),
	  _rhs( rhs ),
	  _current_sum( 0 ),
	  _higher_rhs( 0 ),
	  _copy_variables( variables )
{ }

double LinearEq::required_error( const vector< Variable >& variables ) const
{
	std::copy( variables.begin(), variables.end(), _copy_variables.begin() );
	// _current_sum = std::accumulate( variables.begin(), variables.end(), 0, [&](const Variable& v1, const Variable& v2) -> int{ return v1.get_value() + v2.get_value(); } );
	_current_sum = 0;
	for( const Variable& var : variables )
		_current_sum += var.get_value();

	int diff = _rhs - _current_sum;

	if( diff < 0 )
		_higher_rhs = -1;
	else if ( diff > 0 )
		_higher_rhs = 1;
	else
	{
		_higher_rhs = 0;
		return 0.0;
	}
	
	int number_var_to_change = 0;
	int sum = _current_sum;

	// sort by capacity, not by value
	if( _higher_rhs > 0 )
	{
		// ascending order
		std::sort( _copy_variables.begin(), _copy_variables.end(), [&](const auto& v1, const auto& v2){ return (v1.get_domain_max_value() - v1.get_value()) > (v2.get_domain_max_value() - v2.get_value()); } );
		for( int i = 0 ; i < static_cast<int>( variables.size() ) && sum < _rhs ; ++i )
		{
			sum += ( _copy_variables[i].get_domain_max_value() - _copy_variables[i].get_value() );
			++number_var_to_change;
		}
	}
	else // _higher_rhs < 0
	{
		// descending order
		std::sort( _copy_variables.begin(), _copy_variables.end(), [&](const auto& v1, const auto& v2){ return (v1.get_value() - v1.get_domain_min_value()) > (v2.get_value() - v2.get_domain_min_value()); } );
		for( int i = 0 ; i < static_cast<int>( variables.size() ) && sum > _rhs ; ++i )
		{
			sum -= ( _copy_variables[i].get_value() - _copy_variables[i].get_domain_min_value() );
			++number_var_to_change;
		}
	}
				
	return number_var_to_change;
}

double LinearEq::expert_delta_error( const vector<Variable>& variables,
                                     const vector<unsigned int>& variable_indexes,
                                     const vector<int>& candidate_values ) const
{
	int diff = 0;
	int number_var_to_change = 0;
	int sum = _current_sum;

	for( int i = 0 ; i < static_cast<int>( variable_indexes.size() ); ++i )
		diff += ( candidate_values[ i ] - variables[ variable_indexes[i] ].get_value() );

	if( diff == 0 )
		return 0.0;
	
	switch( _higher_rhs )
	{
	case 1:
		for( int i = 0 ; i < static_cast<int>( variables.size() ) && sum + diff < _rhs ; ++i )
		{
			// skip variables with new candidate values
			if( std::find_if( variable_indexes.begin(), variable_indexes.end(), [&](auto index){ return _copy_variables[i].get_id() == variables[index].get_id(); } ) != variable_indexes.end() )
				continue;
			else
			{
				sum += ( _copy_variables[i].get_domain_max_value() - _copy_variables[i].get_value() );
				++number_var_to_change;
			}
		}
		if( sum + diff > _rhs )
			++number_var_to_change;
		break;
		
	case -1:
		for( int i = 0 ; i < static_cast<int>( variables.size() ) && sum + diff > _rhs ; ++i )
		{
			// skip variables with new candidate values
			if( std::find_if( variable_indexes.begin(), variable_indexes.end(), [&](auto index){ return _copy_variables[i].get_id() == variables[index].get_id(); } ) != variable_indexes.end() )
				continue;
			else
			{
				sum -= ( _copy_variables[i].get_value() - _copy_variables[i].get_domain_min_value() );
				++number_var_to_change;
			}
		}
		if( sum + diff < _rhs )
			++number_var_to_change;
		break;
		
	default: // case 0
		if( diff < 0 )
			for( int i = 0 ; i < static_cast<int>( variables.size() ) && sum + diff < _rhs ; ++i )
			{
				// skip variables with new candidate values
				if( std::find_if( variable_indexes.begin(), variable_indexes.end(), [&](auto index){ return _copy_variables[i].get_id() == variables[index].get_id(); } ) != variable_indexes.end() )
					continue;
				else
				{
					sum += ( _copy_variables[i].get_domain_max_value() - _copy_variables[i].get_value() );
					++number_var_to_change;
				}
			}		
		else // diff > 0
			for( int i = 0 ; i < static_cast<int>( variables.size() ) && sum + diff > _rhs ; ++i )
			{
				// skip variables with new candidate values
				if( std::find_if( variable_indexes.begin(), variable_indexes.end(), [&](auto index){ return _copy_variables[i].get_id() == variables[index].get_id(); } ) != variable_indexes.end() )
					continue;
				else
				{
					sum -= ( _copy_variables[i].get_value() - _copy_variables[i].get_domain_min_value() );
					++number_var_to_change;
				}
			}
	}
	
	return number_var_to_change - this->get_current_error();
} 

void LinearEq::update_constraint( const vector<Variable>& variables, unsigned int variable_index, int new_value ) 
{
	auto iterator = std::find_if( _copy_variables.begin(), _copy_variables.end(), [&](const auto& var){ return var.get_id() == variables[ variable_index ].get_id(); } );
	_current_sum += ( new_value - iterator->get_value() );
	iterator->set_value( new_value );
	int diff = _rhs - _current_sum;

	if( diff < 0 )
		_higher_rhs = -1;
	else if ( diff > 0 )
		_higher_rhs = 1;
	else
		_higher_rhs = 0;

	if( _higher_rhs >= 0 )
		std::sort( _copy_variables.begin(), _copy_variables.end(), [&](const auto& v1, const auto& v2){ return (v1.get_domain_max_value() - v1.get_value()) > (v2.get_domain_max_value() - v2.get_value()); } );
	else 
		std::sort( _copy_variables.begin(), _copy_variables.end(), [&](const auto& v1, const auto& v2){ return (v1.get_value() - v1.get_domain_min_value()) > (v2.get_value() - v2.get_domain_min_value()); } );
}
