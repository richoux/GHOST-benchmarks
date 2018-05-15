#include <algorithm>

#include "objectives_target.hpp"
#include "damageFunctions.hpp"

///////////////
// MaxDamage //
///////////////
MaxDamage::MaxDamage( const vector< UnitData >& my_army,
		      const vector< UnitData >& enemy_army )
  : _my_army(my_army),
    _enemy_army(enemy_army)
{ }

double MaxDamage::required_cost( const vector< Variable >& vec_variables ) const
{
  double cost = 0.;

  for( int i = 0; i < vec_variables.size(); ++i )
  {
    auto& vec_costs = compute_my_shoot_damage( i, vec_variables );
    for( auto& c : vec_costs )
      cost += c;
  }

  return -cost;
}

/////////////
// MaxKill //
/////////////
MaxKill::MaxKill( const vector< UnitData >& my_army,
		  const vector< UnitData >& enemy_army )
  : _my_army(my_army),
    _enemy_army(enemy_army)
{ }

double MaxKill::required_cost( const vector< Variable >& vec_variables ) const
{
  vector<double> total_damage( _enemy_army.size(), 0. );
  
  for( int i = 0; i < vec_variables.size(); ++i )
  {
    auto& vec_costs = compute_my_shoot_damage( i, vec_variables );
    for( int i = 0; i < vec_costs.size(); ++i )
      total_damage[ i ] += vec_costs[ i ];
  }

  int count = 0;
  for( int i = 0; i < _enemy_army.size(); ++i )
  {
    if( total_damage[ i ] >= _enemy_army[ i ].hp )
      ++count;
  }
    
  return -count;
}

/////////////////
// MinOverkill //
/////////////////
MinOverkill::MinOverkill( const vector< UnitData >& my_army,
			  const vector< UnitData >& enemy_army )
  : _my_army(my_army),
    _enemy_army(enemy_army)
{ }

double MinOverkill::required_cost( const vector< Variable >& vec_variables ) const
{
  double cost = 0.;

  vector<double> total_damage( _enemy_army.size(), 0. );
  
  for( int i = 0; i < vec_variables.size(); ++i )
  {
    auto& vec_costs = compute_my_shoot_damage( i, vec_variables );
    for( int i = 0; i < vec_costs.size(); ++i )
      total_damage[ i ] += vec_costs[ i ];
  }

  int count = 0;
  for( int i = 0; i < _enemy_army.size(); ++i )
  {
    if( total_damage[ i ] >= _enemy_army[ i ].hp )
      ++count;

    cost += ( total_damage[ i ] - ( std::max( 0, total_damage[ i ] - _enemy_army[ i ].hp ) ) );
  }

  // (MaxDamage without overkill) * (number of kills + 1)
  return -cost * ( count + 1 );
}
