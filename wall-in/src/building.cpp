#include "building.hpp"

Building::Building( int width, int height, int top, int right, int bottom, int left, Race race, int tree_depth, std::string name )
	: _width( width ),
	  _height( height ),
	  _gap_top( top ), 
	  _gap_right( right ), 
	  _gap_bottom( bottom ), 
	  _gap_left( left ),
	  _race( race ),
	  _tree_depth( tree_depth ),
	  _name( name )
{ }

// std::ostream& operator<<( std::ostream& os, const Building& b )
// {
// 	return os
// 		<< "Race: " <<  b.get_race_string() << "\n"
// 		<< "Name: " << b._name << "\n"
// 		<< "Tree depth: " << b._tree_depth << "\n"
// 		<< "Width: " <<  b._width << "\n"
// 		<< "Height: " <<  b._height << "\n"
// 		<< "Gap Top: " <<  b._gap_top << "\n"
// 		<< "Gap Right: " <<  b._gap_right << "\n"
// 		<< "Gap Bottom: " <<  b._gap_bottom << "\n"
// 		<< "Gap Left: " <<  b._gap_left << "\n"
// 		<< "-------\n";
// }

Building make_building( BuildingType type )
{
  switch( type )
  {
    // Terran
  case BuildingType::Academy:
		return Building(3, 2, 0, 3, 7, 8, Terran, 2, "Terran_Academy");
  case BuildingType::Armory:
		return Building(3, 2, 0, 0, 9, 0, Terran, 3, "Terran_Armory");
  case BuildingType::Barracks:
		return Building(4, 3, 8, 7, 15, 16, Terran, 1, "Terran_Barracks");
  case BuildingType::Bunker:
		return Building(3, 2, 8, 15, 15, 16, Terran, 2, "Terran_Bunker");
  case BuildingType::CommandCenter:
		return Building(4, 3, 7, 5, 6, 6, Terran, 0, "Terran_Command_Center");
  case BuildingType::EngineeringBay:
		return Building(4, 3, 16, 15, 19, 16, Terran, 1, "Terran_Engineering_Bay");
  case BuildingType::Factory:
		return Building(4, 3, 8, 7, 7, 8, Terran, 2, "Terran_Factory");
  case BuildingType::MissileTurret:
		return Building(2, 2, 0, 15, 15, 16, Terran, 2, "Terran_Missile_Turret");
  case BuildingType::ScienceFacility:
		return Building(4, 3, 10, 15, 9, 16, Terran, 4, "Terran_Science_Facility");
  case BuildingType::Starport:
		return Building(4, 3, 8, 15, 9, 16, Terran, 3, "Terran_Starport");
  case BuildingType::SupplyDepot:
		return Building(3, 2, 10, 9, 5, 10, Terran, 0, "Terran_Supply_Depot");
  default:
    throw 0;
  }
}
