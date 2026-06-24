#include <numeric>
#include <memory>

#include "builder_target.hpp"
#include "constraint_shootable.hpp"
#include "objectives_target.hpp"

BuilderTarget::BuilderTarget()
	: ModelBuilder()
{ }

void BuilderTerran::declare_variables()
{
	create_variable( 0, _supply + 1, "Marine" );
	create_variable( 0 , _supply + 1, "Firebat" );
	create_variable( 0, _supply + 1, "Ghost" );
	create_variable( 0, _supply / 2 + 1, "Vulture" );
	create_variable( 0, _supply / 2 + 1, "SiegeTankTankMode" );
	create_variable( 0, _supply / 2 + 1, "SiegeTankSiegeMode" );
	create_variable( 0, _supply / 2 + 1, "Goliath" );
	create_variable( 0, _supply / 2 + 1, "Wraith" );
	create_variable( 0, _supply / 6 + 1, "BattleCruiser" );
}

void BuilderTarget::declare_constraints()
{
	constraints.emplace_back( make_shared<Shootable>( variables, my_army, enemies ) );
}

void BuilderTarget::declare_objective()
{
	objective = make_shared<MaxDamageMaxKill>( my_army, enemies );
}

void BuilderTerran::declare_auxiliary_data()
{
	vector<UnitData> unit_data;
	unit_data.emplace_back( 50, 0, 1., (6.0/15)*24 );
	unit_data.emplace_back( 50, 25, 1., (16.0/22)*24 );
	unit_data.emplace_back( 25, 75, 1., (10.0/22)*24 );
	unit_data.emplace_back( 75, 0, 2., (20.0/30)*24 );
	unit_data.emplace_back( 150, 100, 2., (30.0/37)*24 );
	unit_data.emplace_back( 150, 100, 2., (70.0/75)*24 );
	unit_data.emplace_back( 100, 50, 2., (12.0/22)*24 );
	unit_data.emplace_back( 150, 100, 2., (8.0/30)*24 );
	unit_data.emplace_back( 400, 300, 6., (25.0/30)*24 );
	
	auxiliary_data = make_shared<Data>( variables, unit_data );	
}
