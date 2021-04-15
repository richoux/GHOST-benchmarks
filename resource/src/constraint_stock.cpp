#include <algorithm>
#include <exception>

#include "constraint_stock.hpp"

using namespace std;
using namespace ghost;

Stock::Stock( const vector<Variable>& variables,
              int quantity,
              ResourceType type,
              const vector< UnitData >& unit_data )
	: Constraint( variables ),
    _quantity( quantity ),
    _type( type ),
	  _unit_data( unit_data ),
	  _current_diff( 0 )
{ }

double Stock::required_error( const vector< Variable >& variables ) const
{
  double sum = 0.;
  double cost_value;

  for( int i = 0 ; i < _unit_data.size() ; ++i )
  {
    switch( _type )
    {
    case Mineral:
      cost_value = _unit_data[i].get_mineral();
      break;
    case Gas:
      cost_value = _unit_data[i].get_gas();
      break;
    case Supply:
      cost_value = _unit_data[i].get_supply();
      break;
    default:
      throw 0;
    }
    
    sum += ( variables[i].get_value() * cost_value );
  }

  _current_diff = sum - _quantity;
  
  return std::max( 0., _current_diff );
}

double Stock::expert_delta_error( const vector<Variable>& variables,
                                  const vector<unsigned int>& variable_indexes,
                                  const vector<int>& candidate_values ) const
{
	double diff = _current_diff;
  double cost_value;

	for( int i = 0 ; i < static_cast<int>( variable_indexes.size() ); ++i )
	{
		switch( _type )
		{
    case Mineral:
      cost_value = _unit_data[ variable_indexes[i] ].get_mineral();
      break;
    case Gas:
      cost_value = _unit_data[ variable_indexes[i] ].get_gas();
      break;
    case Supply:
      cost_value = _unit_data[ variable_indexes[i] ].get_supply();
      break;
    default:
      throw 0;
    }

		diff += ( cost_value * ( candidate_values[ i ] - variables[ variable_indexes[i] ].get_value() ) );
	}
	
	return std::max( 0., diff ) - std::max( 0., _current_diff );
} 

void Stock::update_constraint( const vector<Variable>& variables, unsigned int variable_index, int new_value ) 
{
  double cost_value;

  switch( _type )
	{
    case Mineral:
      cost_value = _unit_data[ variable_index ].get_mineral();
      break;
    case Gas:
      cost_value = _unit_data[ variable_index ].get_gas();
      break;
    case Supply:
      cost_value = _unit_data[ variable_index ].get_supply();
      break;
    default:
      throw 0;
    }

  _current_diff += ( cost_value * ( new_value - variables[ variable_index ].get_value() ) );
}

