#include "factoryUnitData.hpp"

UnitData factory_unitData( UnitType type, Coord coordinates )
{
  switch( type )
  {
    // Terran
  case UnitType::Marine:
    return UnitData( "Terran_Marine", 40, 0, Small, 0, 15, 6, Normal, {0, 128}, {0,0,0}, coordinates, false ) ;
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
