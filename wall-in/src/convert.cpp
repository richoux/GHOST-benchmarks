#include "convert.hpp"

int coord_to_index( int row, int column, int width )
{
	return row * width + column;
}
