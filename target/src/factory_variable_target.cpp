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

void make_my_terran( std::vector<UnitType>& vec_type, std::vector<UnitData>& vec_data )
{
  vec_type.clear();
  vec_data.clear();
  
  vec_type.push_back( UnitType::Marine ); // 0
  vec_data.push_back( make_data( UnitType::Marine, {30, 30} ) );
  
  vec_type.push_back( UnitType::Marine ); // 1
  vec_data.push_back( make_data( UnitType::Marine, {50, 30} ) );

  vec_type.push_back( UnitType::Marine ); // 2
  vec_data.push_back( make_data( UnitType::Marine, {70, 30} ) );

  vec_type.push_back( UnitType::Marine ); // 3
  vec_data.push_back( make_data( UnitType::Marine, {90, 30} ) );

  vec_type.push_back( UnitType::Marine ); // 4
  vec_data.push_back( make_data( UnitType::Marine, {110, 30} ) );
    
  vec_type.push_back( UnitType::Goliath ); // 5
  vec_data.push_back( make_data( UnitType::Goliath, {10, 65} ) );

  vec_type.push_back( UnitType::Vulture ); // 6
  vec_data.push_back( make_data( UnitType::Vulture, {50, 65} ) );

  vec_type.push_back( UnitType::Vulture ); // 7
  vec_data.push_back( make_data( UnitType::Vulture, {90, 65} ) );

  vec_type.push_back( UnitType::Goliath ); // 8
  vec_data.push_back( make_data( UnitType::Goliath, {130, 65} ) );

  vec_type.push_back( UnitType::SiegeTankTankMode ); // 9
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {30, 80} ) ); // change coordinates to make reach the SiegeTankSiegeMode. Before: (10, 102)

  vec_type.push_back( UnitType::Ghost ); // 10
  vec_data.push_back( make_data( UnitType::Ghost, {65, 102} ) );

  vec_type.push_back( UnitType::Ghost ); // 11
  vec_data.push_back( make_data( UnitType::Ghost, {75, 102} ) );

  vec_type.push_back( UnitType::SiegeTankTankMode ); // 12
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {110, 80} ) ); // change coordinates to make reach the SiegeTankSiegeMode. Before: (130, 102)

  vec_type.push_back( UnitType::SiegeTankSiegeMode ); // 13
  vec_data.push_back( make_data( UnitType::SiegeTankSiegeMode, {70, 139} ) );
}

void make_enemy_terran( std::vector<UnitData>& vec_data )
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
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {30, -80} ) );
  vec_data.push_back( make_data( UnitType::Ghost, {65, -102} ) );
  vec_data.push_back( make_data( UnitType::Ghost, {75, -102} ) );
  vec_data.push_back( make_data( UnitType::SiegeTankTankMode, {110, -80} ) );
  vec_data.push_back( make_data( UnitType::SiegeTankSiegeMode, {70, -139} ) );
}
