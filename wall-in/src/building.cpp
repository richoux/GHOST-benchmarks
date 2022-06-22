#include "building.hpp"

Building::Building( int width, int height, int top, int right, int bottom, int left, Race race, int tree_depth, string name )
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

ostream& operator<<( ostream& os, const Building& b )
{
	return os
		<< "Race: " <<  b.get_race_string() << endl
		<< "Name: " << b._name << endl
		<< "Tree depth: " << b._tree_depth << endl
		<< "Width: " <<  b._width << endl
		<< "Height: " <<  b._height << endl
		<< "Gap Top: " <<  b._gap_top << endl
		<< "Gap Right: " <<  b._gap_right << endl
		<< "Gap Bottom: " <<  b._gap_bottom << endl
		<< "Gap Left: " <<  b._gap_left << endl
		<< "-------" << endl;
}
