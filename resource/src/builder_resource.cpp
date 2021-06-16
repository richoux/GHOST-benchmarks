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
	variables.emplace_back( "Marine", 0, _supply + 1 );
	variables.emplace_back( "Firebat", 0 , _supply + 1);
	variables.emplace_back( "Ghost", 0, _supply + 1);
	variables.emplace_back( "Vulture", 0, _supply / 2 + 1 );
	variables.emplace_back( "SiegeTankTankMode", 0, _supply / 2 + 1 );
	variables.emplace_back( "SiegeTankSiegeMode", 0, _supply / 2 + 1 );
	variables.emplace_back( "Goliath", 0, _supply / 2 + 1 );
	variables.emplace_back( "Wraith", 0, _supply / 2 + 1 );
	variables.emplace_back( "BattleCruiser", 0, _supply / 6 + 1 );
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
	variables.emplace_back( "Zealot", 0, _supply / 2 + 1 );
	variables.emplace_back( "Dragoon", 0, _supply / 2 + 1 );
	variables.emplace_back( "DarkTemplar", 0, _supply / 2 + 1 );
	variables.emplace_back( "Reaver", 0, _supply / 4 + 1 );
	variables.emplace_back( "Scout", 0, _supply / 3 + 1 );
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
	variables.emplace_back( "Zergling", 0, _supply * 2 + 1 );
	variables.emplace_back( "Hydralisk", 0, _supply + 1 );
	variables.emplace_back( "Lurker", 0, _supply / 2 + 1);
	variables.emplace_back( "Ultralisk", 0, _supply / 4 + 1 );
	variables.emplace_back( "Mutalisk", 0, _supply / 2 + 1 );
	variables.emplace_back( "Guardian", 0, _supply / 2 + 1 );
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
