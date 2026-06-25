#include <numeric>
#include <memory>

#include "builder_target.hpp"
#include "constraint_shootable.hpp"
#include "objectives_target.hpp"

BuilderTarget::BuilderTarget( const std::vector<UnitType>& my_units,
                              int number_enemies,
                              const std::vector<UnitData>& my_army,
                              const std::vector<UnitData>& enemies )
	: ModelBuilder(),
	  _my_units(my_units),
	  _number_enemies(number_enemies),
	  _my_army(my_army),
	  _enemies(enemies)
{ }

void BuilderTerran::declare_variables()
{
	for( auto& u : _my_units )
		create_variable( u );
}

void BuilderTarget::declare_constraints()
{
	constraints.emplace_back( make_shared<Shootable>( variables, _my_army, _enemies ) );
}

void BuilderTarget::declare_objective()
{
	objective = make_shared<MaxDamageMaxKill>( variables, _my_army, _enemies );
}

void BuilderTarget::create_variable( UnitType type )
{
  switch( type )
  {
    // Terran
  case UnitType::Marine:
	  create_variable( -1, _number_enemies, "Marine" );
  case UnitType::Firebat:
    create_variable( -1, _number_enemies, "Firebat" );
  case UnitType::Ghost:
    create_variable( -1, _number_enemies,"Ghost" );
  case UnitType::Vulture:
    create_variable( -1, _number_enemies, "Vulture" );
  case UnitType::Goliath:
    create_variable( -1, _number_enemies, "Goliath" );
  case UnitType::SiegeTankTankMode:
    create_variable( -1, _number_enemies, "SiegeTankTankMode" );
  case UnitType::SiegeTankSiegeMode:
    create_variable( -1, _number_enemies, "SiegeTankSiegeMode" );
  default:
    throw 0;
  }
}
