#pragma once

#include <vector>
#include <fstream>

#include <ghost/variables.hpp>
#include "building.hpp"

using namespace std;

class Placement : public Grid
{
	vector< vector<int> > _placement;
	vector< Building > _buildings;
	
public:
	Placement( int width, int height, const vector< Building >& buildings );
	Placement( const ifstream& file_data );

	void fill( const vector<Variable>& variables );
	void fill( const vector<int>& positions ) override;

	void add( int building_id, int position );
	bool remove( int building_id, int position );
	
	vector<int> what_is_at( int coordinate ) const;
	vector<int> what_is_at( int x, int y ) const;
};
