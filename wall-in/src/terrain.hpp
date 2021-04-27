#pragma once

#include <vector>
#include <fstream>

using namespace std;

class Terrain : public Grid
{
	vector<bool> _terrain;

public:
	Terrain( int width, int height );
	Terrain( const ifstream& file_data );

	void fill( const vector<int>& linear_terrain ) override;

	bool is_constructible( int coordinate ) const;
	bool is_constructible( int x, int y ) const;
};
