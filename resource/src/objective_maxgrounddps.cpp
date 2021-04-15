#include <numeric>
#include <iostream>

#include "objective_maxgrounddps.hpp"

using namespace std;

MaxGroundDPS::MaxGroundDPS( const vector<Variable>& variables, const vector< UnitData >& unit_data )
	: Objective( "Max Ground DPS", variables ),
    _unit_data( unit_data )
{ }

double MaxGroundDPS::required_cost( const vector< Variable >& vecVariables ) const 
{
  double total = 0.;

  for( int i = 0 ; i < _unit_data.size() ; ++i )
    total += ( vecVariables[i].get_value() * _unit_data[i].get_dps() );
  
  return -total;
}
