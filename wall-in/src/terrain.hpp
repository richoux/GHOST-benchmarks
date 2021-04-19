#pragma once

#include <vector>
#include <fstream>

#include "utils.hpp"

using namespace std;

class Terrain
{
	int _length;
	int _height;

	// /!\ WARNING /!\
	// To exploit prefetching as much as possible, the terrain matrix coordinates
	// starts with the height (y coordinate, or the row number),
	// followed by the length (x coordinate, or the column number)
	// This coordinate management is completely transparent for other classes
	// (manipulating usual (x,y) coordinates)
	vector<vector<bool>> _terrain;

public:
	Terrain( int lenght, int height );
	Terrain( const ifstream& file_data );

	void fill_terrain( vector<int> linear_terrain );

	bool is_constructible( int coordinate );
};
