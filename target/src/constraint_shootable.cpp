#include <algorithm>

#include "contraint_shootable.hpp"

Shootable::Shootable( const vector< Variable >& variables,
		      const vector< UnitData >& my_army,
		      const vector< UnitData >& enemy_army )
  : variables(variables),
    _my_army(my_army),
    _enemy_army(enemy_army)
{ }


double Shootable::required_cost() const
{
  double cost = 0.;

  for( int i = 0; i < variables.size(); ++i )
  {
    auto& my_unit = _my_army[ i ];

    // If the unit is in cooldown, there is nothing to check
    if( my_unit.can_shoot() )
    {
      // No target assigned
      if( variables[ i ] == -1 )
      {
	// if some enemies are alive and within range while having no target assigned,
	// increase the cost by the number of such enemies.
	int count = std::count_if( _enemy_army.begin(),
				   _enemy_army.end(),
				   [&](auto& enemy){ my_unit.is_in_range_and_alive( enemy ) } );
	cost += count;
      }
      else
      {
	auto& target = _enemy_army[ variables[ i ] ];
	
	// If our target is dead or not in range, increment the cost;
	if( !my_unit.is_in_range_and_alive( target ) )
	  ++cost;
      }
    }
  }

  return cost;
}

