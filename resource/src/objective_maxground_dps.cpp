#include <numeric>
#include <iostream>

#include "objective_maxground_dps.hpp"

using namespace std;

MaxGroundDPS::MaxGroundDPS( const vector<Variable>& variables, shared_ptr<AuxiliaryData> auxiliary_data )
	: Objective( variables, true, "Max Ground DPS" ),
	  _unit_data( dynamic_pointer_cast<Data>(auxiliary_data)->unit_data )
{ }

double MaxGroundDPS::required_cost( const vector<Variable*>& vecVariables ) const 
{
  double total = 0.;

  for( int i = 0 ; i < static_cast<int>( _unit_data.size() ) ; ++i )
    total += ( vecVariables[i]->get_value() * _unit_data[i].get_dps() );
  
  return total;
}
