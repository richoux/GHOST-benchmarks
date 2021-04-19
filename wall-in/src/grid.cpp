#include "grid.hpp"

Grid::Grid( int length, int height )
	: _length( length ),
	  _height( height )
{
	_grid.resize( _height );
	for( int y = 0 ; y < _height ; ++y )
		_grid[y].resize( _length, -1 );
}	

Grid::Grid( const ifstream& file_data )
{ }

void Grid::fill_grid( vector<int> linear_grid )
{
	for( int position = 0 ; position < static_cast<int>( linear_grid.size() ) ; ++position )
	{
		auto [x , y] = line2matrix( position, _length );
		_grid[y][x] = linear_grid[ position ];
	}
}

auto Grid::get_size()
{
	struct size { int l, h; };
	return size { _length, _height };
}

int Grid::what_is_at( int coordinate ) const
{
	auto [x , y] = line2matrix( coordinate, _length );
	return _grid[y][x];
}

