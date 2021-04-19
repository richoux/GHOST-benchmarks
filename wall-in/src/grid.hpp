#pragma once

#include <vector>
#include <fstream>

#include "utils.hpp"

using namespace std;

class Grid
{
	int _length;
	int _height;

	// /!\ WARNING /!\
	// To exploit prefetching as much as possible, the grid matrix coordinates
	// starts with the height (y coordinate, or the row number),
	// followed by the length (x coordinate, or the column number)
	// This coordinate management is completely transparent for other classes
	// (manipulating usual (x,y) coordinates)
	vector<vector<int>> _grid;

public:
	Grid( int length, int height );
	Grid( const ifstream& file_data );

	void fill_grid( vector<int> linear_grid );

	auto get_size();
	int what_is_at( int coordinate ) const;
};
