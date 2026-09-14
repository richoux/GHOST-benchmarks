#include <algorithm>
#include <iostream>

#include "objectives_target.hpp"
#include "damageFunctions.hpp"

///////////////
// MaxDamage //
///////////////
MaxDamage::MaxDamage( const std::vector<ghost::Variable>& variables,
                      const std::vector< UnitData >& my_army,
                      const std::vector< UnitData >& enemies )
	: Maximize( variables, "MaxDamage" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MaxDamage::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	double cost = 0.;

	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies );
		for( auto& c : vec_costs )
			cost += c;
	}

	return cost;
}

//////////////////////
// MaxDamageMaxKill //
//////////////////////
MaxDamageMaxKill::MaxDamageMaxKill( const std::vector<ghost::Variable>& variables,
                                    const std::vector< UnitData >& my_army,
                                    const std::vector< UnitData >& enemies )
	: Maximize( variables, "MaxDamageMaxKill" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MaxDamageMaxKill::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	double cost = 0.;

	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies );
		for( auto& c : vec_costs )
			cost += c;
	}

	return cost;
}

int MaxDamageMaxKill::expert_heuristic_value( const std::vector< ghost::Variable* >& variables,
                                              int	var_index,
                                              const std::vector< int >&	possible_values,
                                              randutils::mt19937_rng& rng ) const
{
	int best_choice = -1;
	double best_overkill = 0.;

	vector<double> total_costs( _enemies.size(), 0. );
  
	for( int i = 0; i < variables.size(); ++i )
		if( i != var_index )
		{
			auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies );
			for( int j = 0; j < vec_costs.size(); ++j )
				total_costs[ j ] += vec_costs[ j ];
		}

	for( auto& possible: possible_values )
	{
		auto vec_costs = compute_damage( _my_army[ var_index ], possible, _enemies );

		if( _enemies[ possible ].hp <= ( vec_costs[ possible ] + total_costs[ possible ] ) )
			if( best_choice == -1 || ( _enemies[ possible ].hp - ( vec_costs[ possible ] + total_costs[ possible ] ) ) > best_overkill )
			{
				best_choice = possible;
				best_overkill = _enemies[ possible ].hp - vec_costs[ possible ];
			}
	}

	if( best_choice == -1 )
		return rng.pick( possible_values );
	else
		return best_choice;
}


/////////////
// MaxKill //
/////////////
MaxKill::MaxKill( const std::vector<ghost::Variable>& variables,
                  const std::vector< UnitData >& my_army,
                  const std::vector< UnitData >& enemies )
	: Maximize( variables, "MaxKill" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MaxKill::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	vector<double> total_damage( _enemies.size(), 0. );
  
	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies );
		for( int i = 0; i < vec_costs.size(); ++i )
			total_damage[ i ] += vec_costs[ i ];
	}

	int count = 0;
	for( int i = 0; i < _enemies.size(); ++i )
	{
		if( total_damage[ i ] >= _enemies[ i ].hp )
			++count;
	}
    
	return count;
}

/////////////////
// MinOverkill //
/////////////////
MinOverkill::MinOverkill( const std::vector<ghost::Variable>& variables,
                          const std::vector< UnitData >& my_army,
                          const std::vector< UnitData >& enemies )
	: Minimize( variables, "MinOverkill" ),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

double MinOverkill::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	double cost = 0.;

	vector<double> total_damage( _enemies.size(), 0. );
  
	for( int i = 0; i < variables.size(); ++i )
	{
		auto vec_costs = compute_damage( _my_army[ i ], variables[ i ]->get_value(), _enemies );
		for( int i = 0; i < vec_costs.size(); ++i )
			total_damage[ i ] += vec_costs[ i ];
	}

	for( int i = 0; i < _enemies.size(); ++i )
		if( total_damage[ i ] >= _enemies[ i ].hp )
			cost += total_damage[ i ] - _enemies[ i ].hp;

	return cost;
}
