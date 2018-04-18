#include <numeric>
#include <iostream>

#include "obj_maxgrounddps.hpp"

using namespace std;

MaxGroundDPS::MaxGroundDPS( const vector< UnitData >& unit_data )
  : Objective( "Max Ground DPS" ),
    _unit_data	( unit_data )
{ }

double MaxGroundDPS::required_cost( vector< Variable > *vecVariables ) const 
{
  double total = 0.;

  for( int i = 0 ; i < _unit_data.size() ; ++i )
    total += ( (*vecVariables)[i].get_value() * _unit_data[i].get_dps() );
  
  return -total;

  // return std::accumulate( begin( vecVariables ),
  // 			  end( vecVariables ),
  // 			  0,
  // 			  []( auto& v ){
  // 			    Unit* u = dynamic_cast<Unit*>( v.get() );
  // 			    return v.is_assigned() ? v.get_value() * v.get_dps() : 0;
  // 			  } ); 
}
