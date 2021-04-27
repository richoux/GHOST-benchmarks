#pragma once

#include <vector>

using namespace std;

class Grid
{
protected:
	int width;
	int height;

	int matrix2line( int x, int y ) const;
	
public:
	Grid( int width, int height );
	virtual ~Grid() = default;
	
	virtual void fill( const vector<int>& linear_grid ) = 0;
};
