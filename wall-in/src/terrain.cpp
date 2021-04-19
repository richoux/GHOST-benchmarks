#include "terrain.hpp"

Terrain::Terrain( int lenght, int height )
	: _length( lenght ),
	  _height( height ),
{
	_terrain.resize( height );
	for( int y = 0 ; y < height ; ++y )
		_terrain[y].resize( length, false );
}	

Terrain::Terrain( const ifstream& file_data )
{ }

void Terrain::fill_terrain( vector<int> linear_terrain )
{
	for( int position = 0 ; position < static_cast<int>( linear_terrain.size() ) ; ++position )
	{
		auto [x , y] = line2matrix( position, _length );

		// 0 means unwalkable
		// 1 means walkable but unconstructable
		// 2 means constructable
		if( linear_terrain[ position ] == 2 )
			_terrain[y][x] = true;
		else
			_terrain[y][x] = false;
	}
}

bool Terrain::is_constructible( int coordinate )
{
	auto [x , y] = line2matrix( coordinate, _length );
	return _terrain[y][x];
}

