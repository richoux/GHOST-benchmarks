#include "convert.hpp"

std::vector<bool> make_line_from_grid( const std::vector< std::vector<bool> >& grid )
{
	int height = static_cast<int>( grid.size() );
	int width = static_cast<int>( grid[0].size() );
	std::vector<bool> line( height * width );
	
	for( int r = 0 ; r < height ; ++r )
		for( int c = 0 ; c < width ; ++c )
			line[r * width + c] = grid[r][c];

	return line;
}

int coord_to_index( int row, int column, int width )
{
	return row * width + column;
}
