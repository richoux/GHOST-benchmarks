#include <algorithm>
#include <iostream>

#include "objectives_target.hpp"
#include "damageFunctions.hpp"

///////////////
// MaxDamage //
///////////////
MaxDamage::MaxDamage( const vector< UnitData >& my_army,
		      const vector< UnitData >& enemy_army )
  : Objective( "MaxDamage" ),
    _my_army(my_army),
    _enemy_army(enemy_army)
{ }

double MaxDamage::required_cost( const vector< Variable >& vec_variables ) const
{
  double cost = 0.;

  for( int i = 0; i < vec_variables.size(); ++i )
  {
    auto vec_costs = compute_damage( _my_army[ i ], vec_variables[ i ].get_value(), _enemy_army ); //compute_my_shoot_damage( i, vec_variables );
    for( auto& c : vec_costs )
      cost += c;
  }

  return -cost;
}

//////////////////////
// MaxDamageMaxKill //
//////////////////////
MaxDamageMaxKill::MaxDamageMaxKill( const vector< UnitData >& my_army,
				    const vector< UnitData >& enemy_army )
  : Objective( "MaxDamageMaxKill" ),
    _my_army(my_army),
    _enemy_army(enemy_army)
{ }

double MaxDamageMaxKill::required_cost( const vector< Variable >& vec_variables ) const
{
  double cost = 0.;

  for( int i = 0; i < vec_variables.size(); ++i )
  {
    auto vec_costs = compute_damage( _my_army[ i ], vec_variables[ i ].get_value(), _enemy_army ); //compute_my_shoot_damage( i, vec_variables );
    for( auto& c : vec_costs )
      cost += c;
  }

  return -cost;
}

int MaxDamageMaxKill::expert_heuristic_value( const vector< Variable >&	variables,
					      Variable&			var,
					      const vector< int >&	possible_values ) const
{
  int best_choice = -1;
  double best_overkill = 0.;
  int index = 0;

  vector<double> total_costs( _enemy_army.size(), 0. );
  
  for( int i = 0; i < variables.size(); ++i )
    if( var.get_id() == variables[ i ].get_id() )
    {
      index = i;
      break;
    }
    else
    {
      auto vec_costs = compute_damage( _my_army[ i ], variables[ i ].get_value(), _enemy_army );
      for( int j = 0; j < vec_costs.size(); ++j )
	total_costs[ j ] += vec_costs[ j ];
    }

  for( auto& possible: possible_values )
  {
    auto vec_costs = compute_damage( _my_army[ index ], possible, _enemy_army );

    if( _enemy_army[ possible ].hp <= ( vec_costs[ possible ] + total_costs[ possible ] ) )
      if( best_choice == -1 || ( _enemy_army[ possible ].hp - ( vec_costs[ possible ] + total_costs[ possible ] ) ) > best_overkill )
      {
	best_choice = possible;
	best_overkill = _enemy_army[ possible ].hp - vec_costs[ possible ];
      }
  }

  if( best_choice == -1 )
    return possible_values[ random.get_random_number( possible_values.size() ) ];
  else
    return best_choice;
}


/////////////
// MaxKill //
/////////////
MaxKill::MaxKill( const vector< UnitData >& my_army,
		  const vector< UnitData >& enemy_army )
  : Objective( "MaxKill" ),
    _my_army(my_army),
    _enemy_army(enemy_army)
{ }

double MaxKill::required_cost( const vector< Variable >& vec_variables ) const
{
  vector<double> total_damage( _enemy_army.size(), 0. );
  
  for( int i = 0; i < vec_variables.size(); ++i )
  {
    auto vec_costs = compute_damage( _my_army[ i ], vec_variables[ i ].get_value(), _enemy_army ); //compute_my_shoot_damage( i, vec_variables );
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
  : Objective( "MinOverkill" ),
    _my_army(my_army),
    _enemy_army(enemy_army)
{ }

double MinOverkill::required_cost( const vector< Variable >& vec_variables ) const
{
  double cost = 0.;

  vector<double> total_damage( _enemy_army.size(), 0. );
  
  for( int i = 0; i < vec_variables.size(); ++i )
  {
    auto vec_costs = compute_damage( _my_army[ i ], vec_variables[ i ].get_value(), _enemy_army ); //compute_my_shoot_damage( i, vec_variables );
    for( int i = 0; i < vec_costs.size(); ++i )
      total_damage[ i ] += vec_costs[ i ];
  }

  int count = 0;
  for( int i = 0; i < _enemy_army.size(); ++i )
  {
    if( total_damage[ i ] >= _enemy_army[ i ].hp )
      ++count;

    cost += ( total_damage[ i ] - ( std::max( 0., total_damage[ i ] - _enemy_army[ i ].hp ) ) );
  }

  // (MaxDamage without overkill) * (number of kills + 1)
  return -cost * ( count + 1 );
}
