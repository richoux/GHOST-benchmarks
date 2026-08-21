#pragma once

#include <string>
#include "races.hpp"

enum BuildingType{ Academy,
                   Armory,
                   Barracks,
                   Bunker,
                   CommandCenter,
                   EngineeringBay,
                   Factory,
                   MissileTurret,
                   ScienceFacility,
                   Starport,
                   SupplyDepot };


class Building
{
	int _width;
	int _height;

	int _gap_top;
	int _gap_right;
	int _gap_bottom;
	int _gap_left;

	Race _race;
	int _tree_depth;

	std::string _name;

public:
	Building( int width, int height, int top, int right, int bottom, int left, Race race, int tree_depth, std::string name );
    
	inline int get_width()	const	{ return _width; }
	inline int get_height()	const	{ return _height; }

	inline int get_gap_top()	  const	{ return _gap_top; }
	inline int get_gap_right()	const	{ return _gap_right; }
	inline int get_gap_bottom()	const	{ return _gap_bottom; }
	inline int get_gap_left()	  const	{ return _gap_left; }

	inline Race get_race() const { return _race; }
	inline std::string get_race_string()	const	
	{ 
		switch( _race ) 
		{
		case Terran: return "Terran";
		case Protoss: return "Protoss";
		case Zerg: return "Zerg";
		default: return "Unknown";
		}
	}
    
	inline int get_tree_depth()	const	{ return _tree_depth; }
	inline std::string get_name() const { return _name; }
    
	// friend std::ostream& operator<<( std::ostream&, const Building& );
};


Building make_building( BuildingType type );
