#include <algorithm>

#include "constraint_shootable.hpp"

Shootable::Shootable( const vector< reference_wrapper<Variable> >& variables,
		      const vector< UnitData >& my_army,
		      const vector< UnitData >& enemy_army )
  : Constraint(variables),
    _my_army(my_army),
    _enemy_army(enemy_army)
{ }


double Shootable::required_cost() const
{
  double cost = 0.;

  for( int i = 0; i < variables.size(); ++i )
  {
    auto& my_unit = _my_army[ i ];

    // If the unit is alive and in cooldown, there is nothing to check
    if( my_unit.can_shoot() && my_unit.is_alive() )
    {
      int variable_value = variables[ i ].get().get_value();
      // No target assigned
      if( variable_value == -1 )
      {
	// if some enemies are alive and within range while having no target assigned,
	// increase the cost by the number of such enemies.
	int count = std::count_if( _enemy_army.cbegin(),
				   _enemy_army.cend(),
				   [&](auto& enemy){ return my_unit.is_in_range_and_alive( enemy ); } );
	cost += count;

	// if( i == 13 )
	// {
	//   for( int j = 0; j < _enemy_army.size(); ++j )
	//     if( my_unit.is_in_range_and_alive( _enemy_army[ j ] ) ) 
	//       cout << "==> " << my_unit.name << ":" << variables[ i ].get().get_id() << " could shoot " << _enemy_army[ j ].name << "@" << j << "\n";
	//   cout << "count=" << count << ", cost=" << cost << "\n";
	// }
      }
      else
      {
	auto& target = _enemy_army[ variable_value ];
	
	// If our target is dead or not in range, increment the cost;
	if( !my_unit.is_in_range_and_alive( target ) )
	  ++cost;
      }
    }
  }

  return cost;
}

