#include <algorithm>
#include <iostream>

#include "objectives_target.hpp"
#include "damageFunctions.hpp"

///////////////
// MaxDamage //
///////////////
MaxDamage::MaxDamage( const std::vector< UnitData >& my_army,
                      const std::vector< UnitData >& enemies )
	: Maximize( "MaxDamage" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MaxDamage::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	double cost = 0.;

	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies ); //compute_my_shoot_damage( i, variables );
		for( auto& c : vec_costs )
			cost += c;
	}

	return -cost;
}

//////////////////////
// MaxDamageMaxKill //
//////////////////////
MaxDamageMaxKill::MaxDamageMaxKill( const std::vector< UnitData >& my_army,
                                    const std::vector< UnitData >& enemies )
	: Maximize( "MaxDamageMaxKill" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MaxDamageMaxKill::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	double cost = 0.;

	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies ); //compute_my_shoot_damage( i, variables );
		for( auto& c : vec_costs )
			cost += c;
	}

	return -cost;
}

int MaxDamageMaxKill::expert_heuristic_value( const std::vector< ghost::Variable* >& variables,
                                              int	var_index,
                                              const std::vector< int >&	possible_values,
                                              randutils::mt19937_rng& rng ) const
{
	int best_choice = -1;
	double best_overkill = 0.;
	int index = 0;

	vector<double> total_costs( _enemies.size(), 0. );
  
	for( int i = 0; i < variables.size(); ++i )
		if( var.get_id() == variables[ i ]->get_id() )
		{
			index = i;
			break;
		}
		else
		{
			auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies );
			for( int j = 0; j < vec_costs.size(); ++j )
				total_costs[ j ] += vec_costs[ j ];
		}

	for( auto& possible: possible_values )
	{
		auto vec_costs = compute_damage( _my_army[ index ], possible, _enemies );

		if( _enemies[ possible ].hp <= ( vec_costs[ possible ] + total_costs[ possible ] ) )
			if( best_choice == -1 || ( _enemies[ possible ].hp - ( vec_costs[ possible ] + total_costs[ possible ] ) ) > best_overkill )
			{
				best_choice = possible;
				best_overkill = _enemies[ possible ].hp - vec_costs[ possible ];
			}
	}

	if( best_choice == -1 )
		return possible_values[ rng.get_random_number( possible_values.size() ) ];
	else
		return best_choice;
}


/////////////
// MaxKill //
/////////////
MaxKill::MaxKill( const std::vector< UnitData >& my_army,
                  const std::vector< UnitData >& enemies )
	: Maximize( "MaxKill" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MaxKill::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	vector<double> total_damage( _enemies.size(), 0. );
  
	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ].get_value(), _enemies ); //compute_my_shoot_damage( i, variables );
		for( int i = 0; i < vec_costs.size(); ++i )
			total_damage[ i ] += vec_costs[ i ];
	}

	int count = 0;
	for( int i = 0; i < _enemies.size(); ++i )
	{
		if( total_damage[ i ] >= _enemies[ i ].hp )
			++count;
	}
    
	return -count;
}

/////////////////
// MinOverkill //
/////////////////
MinOverkill::MinOverkill( const std::vector< UnitData >& my_army,
                          const std::vector< UnitData >& enemies )
	: Minimize( "MinOverkill" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MinOverkill::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	double cost = 0.;

	vector<double> total_damage( _enemies.size(), 0. );
  
	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ].get_value(), _enemies ); //compute_my_shoot_damage( i, variables );
		for( int i = 0; i < vec_costs.size(); ++i )
			total_damage[ i ] += vec_costs[ i ];
	}

	int count = 0;
	for( int i = 0; i < _enemies.size(); ++i )
	{
		if( total_damage[ i ] >= _enemies[ i ].hp )
			++count;

		cost += ( total_damage[ i ] - ( std::max( 0., total_damage[ i ] - _enemies[ i ].hp ) ) );
	}

	// (MaxDamage without overkill) * (number of kills + 1)
	return -cost * ( count + 1 );
}
