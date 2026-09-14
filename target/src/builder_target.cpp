#include <numeric>
#include <memory>

#include "builder_target.hpp"
#include "constraint_shootable.hpp"
#include "objectives_target.hpp"

BuilderTarget::BuilderTarget( const std::vector<UnitType>& my_units,
                              const std::vector<UnitData>& my_army,
                              const std::vector<UnitData>& enemies )
	: ModelBuilder(),
	  _my_units(my_units),
	  _my_army(my_army),
	  _enemies(enemies),
	  _number_enemies(enemies.size())
{ }

void BuilderTarget::declare_variables()
{
	for( auto& u : _my_units )
		create_variable_from_type( u );
}

void BuilderTarget::declare_constraints()
{
	constraints.emplace_back( make_shared<Shootable>( variables, _my_army, _enemies ) );
}

void BuilderTarget::declare_objective()
{
	//objective = make_shared<MaxDamageMaxKill>( variables, _my_army, _enemies );
	 objective = make_shared<MaxDamage>( variables, _my_army, _enemies );
	// objective = make_shared<MaxKill>( variables, _my_army, _enemies );
	// objective = make_shared<MinOverkill>( variables, _my_army, _enemies );
}

void BuilderTarget::create_variable_from_type( UnitType type )
{
  switch( type )
  {
    // Terran
  case UnitType::Marine:
	  create_variable( -1, _number_enemies+1, std::string("Marine") );
	  break;
  case UnitType::Firebat:
	  create_variable( -1, _number_enemies+1, std::string("Firebat") );
	  break;
  case UnitType::Ghost:
	  create_variable( -1, _number_enemies+1, std::string("Ghost") );
	  break;
  case UnitType::Vulture:
	  create_variable( -1, _number_enemies+1, std::string("Vulture") );
	  break;
  case UnitType::Goliath:
	  create_variable( -1, _number_enemies+1, std::string("Goliath") );
	  break;
  case UnitType::SiegeTankTankMode:
	  create_variable( -1, _number_enemies+1, std::string("SiegeTankTankMode") );
	  break;
  case UnitType::SiegeTankSiegeMode:
	  create_variable( -1, _number_enemies+1, std::string("SiegeTankSiegeMode") );
	  break;
  default:
    throw 0;
  }
}
