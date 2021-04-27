#include "grid.hpp"

Grid::Grid( int width, int height )
	: width( width ),
	  height( height )
{ }	

int Grid::matrix2line( int x, int y ) const
{
	return y * width + x;
}
