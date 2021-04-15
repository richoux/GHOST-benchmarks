#include <exception>

#include "factory_variable_resource.hpp"

using namespace std; 

Variable make_unit( const UnitType& type, int supplyAvailable, UnitData& data )
{
  switch( type )
  {
    // Protoss
  case UnitType::Zealot:
    data = UnitData( 100, 0, 2., (16.0/22)*24 );
    return Variable( "Zealot", 0, supplyAvailable + 1 );
  case UnitType::Dragoon:
    data = UnitData( 125, 50, 2., (20.0/30)*24 );
    return Variable( "Dragoon", 0, supplyAvailable + 1 );
  case UnitType::DarkTemplar:
    data = UnitData( 125, 100, 2., (40.0/30)*24 );
    return Variable( "DarkTemplar", 0, supplyAvailable + 1 );
  case UnitType::Reaver:
    data = UnitData( 200, 100, 4., (100.0/60)*24 );
    return Variable( "Reaver", 0, supplyAvailable + 1 );
  case UnitType::Scout:
    data = UnitData( 275, 125, 3., (8.0/30)*24 );
    return Variable( "Scout", 0, supplyAvailable + 1 );

    // Terran
  case UnitType::Marine:
    data = UnitData( 50, 0, 1., (6.0/15)*24 );
    return Variable( "Marine", 0, supplyAvailable + 1 );
  case UnitType::Firebat:
    data = UnitData( 50, 25, 1., (16.0/22)*24 );
    return Variable( "Firebat", 0, supplyAvailable + 1 );
  case UnitType::Ghost:
    data = UnitData( 25, 75, 1., (10.0/22)*24 );
    return Variable( "Ghost", 0, supplyAvailable + 1 );
  case UnitType::Vulture:
    data = UnitData( 75, 0, 2., (20.0/30)*24 );
    return Variable( "Vulture", 0, supplyAvailable + 1 );
  case UnitType::SiegeTankTankMode:    
    data = UnitData( 150, 100, 2., (30.0/37)*24 );
    return Variable( "SiegeTankTankMode", 0, supplyAvailable + 1 );
  case UnitType::SiegeTankSiegeMode:
    data = UnitData( 150, 100, 2., (70.0/75)*24 );
    return Variable( "SiegeTankSiegeMode", 0, supplyAvailable + 1 );
  case UnitType::Goliath:
    data = UnitData( 100, 50, 2., (12.0/22)*24 );
    return Variable( "Goliath", 0, supplyAvailable + 1 );
  case UnitType::Wraith:
    data = UnitData( 150, 100, 2., (8.0/30)*24 );
    return Variable( "Wraith", 0, supplyAvailable + 1 );
  case UnitType::BattleCruiser:
    data = UnitData( 400, 300, 6., (25.0/30)*24 );
    return Variable( "BattleCruiser", 0, supplyAvailable + 1 );

    // Zerg
  case UnitType::Zergling:
    data = UnitData( 25, 0, 0.5, 15.0 );
    return Variable( "Zergling", 0, supplyAvailable + 1 );
  case UnitType::Hydralisk:
    data = UnitData( 75, 25, 1., (10.0/15)*24 );
    return Variable( "Hydralisk", 0, supplyAvailable + 1 );
  case UnitType::Lurker:
    data = UnitData( 75, 25, 2., (20.0/37)*24 );
    return Variable( "Lurker", 0, supplyAvailable + 1 );
  case UnitType::Ultralisk:
    data = UnitData( 200, 200, 4., (20.0/15)*24 );
    return Variable( "Ultralisk", 0, supplyAvailable + 1 );
  case UnitType::Mutalisk:
    data = UnitData( 100, 100, 2., (9.0/30)*24 );
    return Variable( "Mutalisk", 0, supplyAvailable + 1 );
  case UnitType::Guardian:
    data = UnitData( 100, 100, 2., (20.0/30)*24 );
    return Variable( "Guardian", 0, supplyAvailable + 1 );

  default:
    throw 0;
  }
}

void make_protoss( int supplyAvailable, vector<Variable>& vec_var, vector<UnitData>& vec_data )
{
  vec_var.clear();
  vec_data.clear();
  UnitData ud;
  
  vec_var.push_back( make_unit( UnitType::Zealot, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Dragoon, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::DarkTemplar, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Reaver, supplyAvailable / 4, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Scout, supplyAvailable / 3, ud ) );
  vec_data.push_back( ud );
}

void make_terran( int supplyAvailable, vector<Variable>& vec_var, vector<UnitData>& vec_data )
{
  vec_var.clear();
  vec_data.clear();
  UnitData ud;

  vec_var.push_back( make_unit( UnitType::Marine, supplyAvailable, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Firebat, supplyAvailable, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Ghost, supplyAvailable, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Vulture, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::SiegeTankTankMode, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::SiegeTankSiegeMode, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Goliath, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Wraith, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::BattleCruiser, supplyAvailable / 6, ud ) );
  vec_data.push_back( ud );
}

void make_zerg( int supplyAvailable, vector<Variable>& vec_var, vector<UnitData>& vec_data )
{
  vec_var.clear();
  vec_data.clear();
  UnitData ud;

  vec_var.push_back( make_unit( UnitType::Zergling, supplyAvailable * 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Hydralisk, supplyAvailable, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Lurker, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Ultralisk, supplyAvailable / 4, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Mutalisk, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
  vec_var.push_back( make_unit( UnitType::Guardian, supplyAvailable / 2, ud ) );
  vec_data.push_back( ud );
}
