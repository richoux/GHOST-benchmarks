#include <exception>

#include "factory_unit.hpp"

using namespace std; 

void make_unit( const UnitType& type, int supplyAvailable, Variable& var, UnitData& data )
{
  switch( type )
  {
    // Protoss
  case UnitType::Zealot:
    var = Variable( "Zealot", "p_z", supplyAvailable + 1, 0 );
    data = UnitData( 100, 0, 2., (16.0/22)*24 );
    break;
  case UnitType::Dragoon:
    var = Variable( "Dragoon", "p_d", supplyAvailable + 1, 0 );
    data = UnitData( 125, 50, 2., (20.0/30)*24 );
    break;
  case UnitType::DarkTemplar:
    var = Variable( "DarkTemplar", "p_dt", supplyAvailable + 1, 0 );
    data = UnitData( 125, 100, 2., (40.0/30)*24 );
    break;
  case UnitType::Reaver:
    var = Variable( "Reaver", "p_r", supplyAvailable + 1, 0 );
    data = UnitData( 200, 100, 4., (100.0/60)*24 );
    break;
  case UnitType::Scout:
    var = Variable( "Scout", "p_s", supplyAvailable + 1, 0 );
    data = UnitData( 275, 125, 3., (8.0/30)*24 );
    break;

    // Terran
  case UnitType::Marine:
    var = Variable( "Marine", "t_m", supplyAvailable + 1, 0 );
    data = UnitData( 50, 0, 1., (6.0/15)*24 );
    break;
  case UnitType::Firebat:
    var = Variable( "Firebat", "t_f", supplyAvailable + 1, 0 );
    data = UnitData( 50, 25, 1., (16.0/22)*24 );
    break;
  case UnitType::Ghost:
    var = Variable( "Ghost", "t_gh", supplyAvailable + 1, 0 );
    data = UnitData( 25, 75, 1., (10.0/22)*24 );
    break;
  case UnitType::Vulture:
    var = Variable( "Vulture", "t_v", supplyAvailable + 1, 0 );
    data = UnitData( 75, 0, 2., (20.0/30)*24 );
    break;
  case UnitType::SiegeTankTankMode:
    var = Variable( "SiegeTankTankMode", "t_tm", supplyAvailable + 1, 0 );
    data = UnitData( 150, 100, 2., (30.0/37)*24 );
    break;
  case UnitType::SiegeTankSiegeMode:
    var = Variable( "SiegeTankSiegeMode", "t_sm", supplyAvailable + 1, 0 );
    data = UnitData( 150, 100, 2., (70.0/75)*24 );
    break;
  case UnitType::Goliath:
    var = Variable( "Goliath", "t_go", supplyAvailable + 1, 0 );
    data = UnitData( 100, 50, 2., (12.0/22)*24 );
    break;
  case UnitType::Wraith:
    var = Variable( "Wraith", "t_w", supplyAvailable + 1, 0 );
    data = UnitData( 150, 100, 2., (8.0/30)*24 );
    break;
  case UnitType::BattleCruiser:
    var = Variable( "BattleCruiser", "t_b", supplyAvailable + 1, 0 );
    data = UnitData( 400, 300, 6., (25.0/30)*24 );
    break;

    // Zerg
  case UnitType::Zergling:
    var = Variable( "Zergling", "z_z", supplyAvailable + 1, 0 );
    data = UnitData( 25, 0, 0.5, 15.0 );
    break;
  case UnitType::Hydralisk:
    var = Variable( "Hydralisk", "z_h", supplyAvailable + 1, 0 );
    data = UnitData( 75, 25, 1., (10.0/15)*24 );
    break;
  case UnitType::Lurker:
    var = Variable( "Lurker", "z_l", supplyAvailable + 1, 0 );
    data = UnitData( 75, 25, 2., (20.0/37)*24 );
    break;
  case UnitType::Ultralisk:
    var = Variable( "Ultralisk", "z_u", supplyAvailable + 1, 0 );
    data = UnitData( 200, 200, 4., (20.0/15)*24 );
    break;
  case UnitType::Mutalisk:
    var = Variable( "Mutalisk", "z_m", supplyAvailable + 1, 0 );
    data = UnitData( 100, 100, 2., (9.0/30)*24 );
    break;
  case UnitType::Guardian:
    var = Variable( "Guardian", "z_g", supplyAvailable + 1, 0 );
    data = UnitData( 100, 100, 2., (20.0/30)*24 );
    break;

  default:
    throw 0;
  }
}

void make_protoss( int supplyAvailable, vector<Variable>& vec_var, vector<UnitData>& vec_data )
{
  vec_var.reserve(5);
  vec_data.reserve(5);
  
  make_unit( UnitType::Zealot, supplyAvailable / 2, vec_var[0], vec_data[0] );
  make_unit( UnitType::Dragoon, supplyAvailable / 2, vec_var[1], vec_data[1] );
  make_unit( UnitType::DarkTemplar, supplyAvailable / 2, vec_var[2], vec_data[2] );
  make_unit( UnitType::Reaver, supplyAvailable / 4, vec_var[3], vec_data[3] );
  make_unit( UnitType::Scout, supplyAvailable / 3, vec_var[4], vec_data[4] );
}

void make_terran( int supplyAvailable, vector<Variable>& vec_var, vector<UnitData>& vec_data )
{
  vec_var.reserve(9);
  vec_data.reserve(9);

  make_unit( UnitType::Marine, supplyAvailable, vec_var[0], vec_data[0] );
  make_unit( UnitType::Firebat, supplyAvailable, vec_var[1], vec_data[1] );
  make_unit( UnitType::Ghost, supplyAvailable, vec_var[2], vec_data[2] );
  make_unit( UnitType::Vulture, supplyAvailable / 2, vec_var[3], vec_data[3] );
  make_unit( UnitType::SiegeTankTankMode, supplyAvailable / 2, vec_var[4], vec_data[4] );
  make_unit( UnitType::SiegeTankSiegeMode, supplyAvailable / 2, vec_var[5], vec_data[5] );
  make_unit( UnitType::Goliath, supplyAvailable / 2, vec_var[6], vec_data[6] );
  make_unit( UnitType::Wraith, supplyAvailable / 2, vec_var[7], vec_data[7] );
  make_unit( UnitType::BattleCruiser, supplyAvailable / 6, vec_var[8], vec_data[8] );
}

void make_zerg( int supplyAvailable, vector<Variable>& vec_var, vector<UnitData>& vec_data )
{
  vec_var.reserve(6);
  vec_data.reserve(6);

  make_unit( UnitType::Zergling, supplyAvailable * 2, vec_var[0], vec_data[0] );
  make_unit( UnitType::Hydralisk, supplyAvailable, vec_var[1], vec_data[1] );
  make_unit( UnitType::Lurker, supplyAvailable / 2, vec_var[2], vec_data[2] );
  make_unit( UnitType::Ultralisk, supplyAvailable / 4, vec_var[3], vec_data[3] );
  make_unit( UnitType::Mutalisk, supplyAvailable / 2, vec_var[4], vec_data[4] );
  make_unit( UnitType::Guardian, supplyAvailable / 2, vec_var[5], vec_data[5] );
}
