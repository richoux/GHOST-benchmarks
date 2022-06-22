#include <numeric>
#include <memory>

#include "builder_resource.hpp"

BuilderResource::BuilderResource( int supply )
	: ModelBuilder(),
	  _supply( supply )
{ }
	
void BuilderResource::declare_constraints()
{
	constraints.emplace_back( make_shared<Stock>( variables, 20000, ResourceType::Mineral, auxiliary_data ) );
	constraints.emplace_back( make_shared<Stock>( variables, 14000, ResourceType::Gas, auxiliary_data ) );
	constraints.emplace_back( make_shared<Stock>( variables, 380, ResourceType::Supply, auxiliary_data ) );
}

void BuilderResource::declare_objective()
{
	objective = make_shared<MaxGroundDPS>( variables, auxiliary_data );
}

BuilderTerran::BuilderTerran( int supply )
	: BuilderResource( supply )
{ }

void BuilderTerran::declare_variables()
{
	variables.emplace_back( 0, _supply + 1, "Marine" );
	variables.emplace_back( 0 , _supply + 1, "Firebat" );
	variables.emplace_back( 0, _supply + 1, "Ghost" );
	variables.emplace_back( 0, _supply / 2 + 1, "Vulture" );
	variables.emplace_back( 0, _supply / 2 + 1, "SiegeTankTankMode" );
	variables.emplace_back( 0, _supply / 2 + 1, "SiegeTankSiegeMode" );
	variables.emplace_back( 0, _supply / 2 + 1, "Goliath" );
	variables.emplace_back( 0, _supply / 2 + 1, "Wraith" );
	variables.emplace_back( 0, _supply / 6 + 1, "BattleCruiser" );
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

BuilderProtoss::BuilderProtoss( int supply )
	: BuilderResource( supply )
{ }

void BuilderProtoss::declare_variables()
{
	variables.emplace_back( 0, _supply / 2 + 1, "Zealot" );
	variables.emplace_back( 0, _supply / 2 + 1, "Dragoon" );
	variables.emplace_back( 0, _supply / 2 + 1, "DarkTemplar" );
	variables.emplace_back( 0, _supply / 4 + 1, "Reaver" );
	variables.emplace_back( 0, _supply / 3 + 1, "Scout" );
}

void BuilderProtoss::declare_auxiliary_data()
{
	vector<UnitData> unit_data;
	unit_data.emplace_back( 100, 0, 2., (16.0/22)*24 );
	unit_data.emplace_back( 125, 50, 2., (20.0/30)*24 );
	unit_data.emplace_back( 125, 100, 2., (40.0/30)*24 );
	unit_data.emplace_back( 200, 100, 4., (100.0/60)*24 );
	unit_data.emplace_back( 275, 125, 3., (8.0/30)*24 );
	
	auxiliary_data = make_shared<Data>( variables, unit_data );
}

BuilderZerg::BuilderZerg( int supply )
	: BuilderResource( supply )
{ }

void BuilderZerg::declare_variables()
{
	variables.emplace_back( 0, _supply * 2 + 1, "Zergling" );
	variables.emplace_back( 0, _supply + 1, "Hydralisk" );
	variables.emplace_back( 0, _supply / 2 + 1, "Lurker" );
	variables.emplace_back( 0, _supply / 4 + 1, "Ultralisk" );
	variables.emplace_back( 0, _supply / 2 + 1, "Mutalisk" );
	variables.emplace_back( 0, _supply / 2 + 1, "Guardian" );
}

void BuilderZerg::declare_auxiliary_data()
{
	vector<UnitData> unit_data;
	unit_data.emplace_back( 25, 0, 0.5, 15.0 );
	unit_data.emplace_back( 75, 25, 1., (10.0/15)*24 );
	unit_data.emplace_back( 75, 25, 2., (20.0/37)*24 );
	unit_data.emplace_back( 200, 200, 4., (20.0/15)*24 );
	unit_data.emplace_back( 100, 100, 2., (9.0/30)*24 );
	unit_data.emplace_back( 100, 100, 2., (20.0/30)*24 );
	
	auxiliary_data = make_shared<Data>( variables, unit_data );	
}
