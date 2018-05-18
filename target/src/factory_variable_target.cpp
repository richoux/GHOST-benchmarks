#include "factory_variable_target.hpp"

UnitData make_data( UnitType type, Coord coordinates )
{
  switch( type )
  {
    // Terran
  case UnitType::Marine:
    return UnitData( "Terran_Marine", 40, 0, Small, 0, 15, 6, Normal, {0, 128}, {0,0,0}, coordinates, false );
  case UnitType::Firebat:
    return UnitData( "Terran_Firebat", 50, 1, Small, 0, 22, 16, Concussive, {0, 32}, {15,20,25}, coordinates, true, true );
  case UnitType::Ghost:
    return UnitData( "Terran_Ghost", 45, 0, Small, 0, 22, 10, Concussive, {0, 244}, {0,0,0}, coordinates, false );
  case UnitType::Vulture:
    return UnitData( "Terran_Vulture", 80, 0, Medium, 0, 30, 20, Concussive, {0, 160}, {0,0,0}, coordinates, false );
  case UnitType::Goliath:
    return UnitData( "Terran_Goliath", 125, 1, Large, 0, 22, 12, Normal, {0, 192}, {0,0,0}, coordinates, false );
  case UnitType::SiegeTankTankMode:
    return UnitData( "Terran_Siege_Tank_Tank_Mode", 150, 1, Large, 0, 37, 30, Explosive, {0, 224}, {0,0,0}, coordinates, false );
  case UnitType::SiegeTankSiegeMode:
    return UnitData( "Terran_Siege_Tank_Siege_Mode", 150, 1, Large, 0, 75, 70, Explosive, {64, 384}, {10,25,40}, coordinates, true );
  default:
    throw 0;
  }
}

Variable make_unit( UnitType type, int domain_size )
{
  switch( type )
  {
    // Terran
  case UnitType::Marine:
    return Variable( "Marine", "t_m", -1, domain_size );
  case UnitType::Firebat:
    return Variable( "Firebat", "t_f", -1, domain_size );
  case UnitType::Ghost:
    return Variable( "Ghost", "t_gh", -1, domain_size );
  case UnitType::Vulture:
    return Variable( "Vulture", "t_v", -1, domain_size );
  case UnitType::Goliath:
    return Variable( "Goliath", "t_go", -1, domain_size );
  case UnitType::SiegeTankTankMode:
    return Variable( "SiegeTankTankMode", "t_tm", -1, domain_size );
  case UnitType::SiegeTankSiegeMode:
    return Variable( "SiegeTankSiegeMode", "t_sm", -1, domain_size );
  default:
    throw 0;
  }
}

void make_my_terran( vector<Variable>& vec_var, vector<UnitData>& vec_data )
{
  vec_var.clear();
  vec_data.clear();
  
  vec_var.push_back( make_unit( UnitType::Marine, 14 ) ); // 0
  vec_data.push_back( make_data( UnitType::Marine, {30, 30} ) );
  
  vec_var.push_back( make_unit( UnitType::Marine, 14 ) ); // 1
  vec_data.push_back( make_data( UnitType::Marine, {50, 30} ) );

  vec_var.push_back( make_unit( UnitType::Marine, 14 ) ); // 2
  vec_data.push_back( make_data( UnitType::Marine, {70, 30} ) );

  vec_var.push_back( make_unit( UnitType::Marine, 14 ) ); // 3
  vec_data.push_back( make_data( UnitType::Marine, {90, 30} ) );

  vec_var.push_back( make_unit( UnitType::Marine, 14 ) ); // 4
  vec_data.push_back( make_data( UnitType::Marine, {110, 30} ) );
    
  vec_var.push_back( make_unit( UnitType::Goliath, 14 ) ); // 5
  vec_data.push_back( make_data( UnitType::Goliath, {10, 65} ) );

  vec_var.push_back( make_unit( UnitType::Vulture, 14 ) ); // 6
  vec_data.push_back( make_data( UnitType::Vulture, {50, 65} ) );

  vec_var.push_back( make_unit( UnitType::Vulture, 14 ) ); // 7
  vec_data.push_back( make_data( UnitType::Vulture, {90, 65} ) );

  vec_var.push_back( make_unit( UnitType::Goliath, 14 ) ); // 8
  vec_data.push_back( make_data( UnitType::Goliath, {130, 65} ) );

  vec_var.push_back( make_unit( UnitType::SiegeTankTankMode, 14 ) ); // 9
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {10, 102} ) );

  vec_var.push_back( make_unit( UnitType::Ghost, 14 ) ); // 10
  vec_data.push_back( make_data( UnitType::Ghost, {65, 102} ) );

  vec_var.push_back( make_unit( UnitType::Ghost, 14 ) ); // 11
  vec_data.push_back( make_data(  UnitType::Ghost, {75, 102} ) );

  vec_var.push_back( make_unit( UnitType::SiegeTankTankMode, 14 ) ); // 12
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {130, 102} ) );

  vec_var.push_back( make_unit( UnitType::SiegeTankSiegeMode, 14 ) ); // 13
  vec_data.push_back( make_data( UnitType::SiegeTankSiegeMode, {70, 139} ) );
}

void make_enemy_terran( vector<UnitData>& vec_data )
{
  vec_data.clear();

  vec_data.push_back( make_data( UnitType::Marine, {30, -30} ) );
  vec_data.push_back( make_data( UnitType::Marine, {50, -30} ) );
  vec_data.push_back( make_data( UnitType::Marine, {70, -30} ) );
  vec_data.push_back( make_data( UnitType::Marine, {90, -30} ) );
  vec_data.push_back( make_data( UnitType::Marine, {110, -30} ) );
  vec_data.push_back( make_data( UnitType::Goliath, {10, -65} ) );
  vec_data.push_back( make_data( UnitType::Vulture, {50, -65} ) );
  vec_data.push_back( make_data( UnitType::Vulture, {90, -65} ) );
  vec_data.push_back( make_data( UnitType::Goliath, {130, -65} ) );
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {10, -102} ) );
  vec_data.push_back( make_data( UnitType::Ghost, {65, -102} ) );
  vec_data.push_back( make_data(  UnitType::Ghost, {75, -102} ) );
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {130, -102} ) );
  vec_data.push_back( make_data( UnitType::SiegeTankSiegeMode, {70, -139} ) );
}
