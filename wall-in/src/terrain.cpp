#include <algorithm>

#include "terrain.hpp"

Terrain::Terrain( int width, int height )
	: Grid( width, height ),
	  _terrain( vector<bool>( width * height, false ) )
{ }	

Terrain::Terrain( const ifstream& file_data )
{ }

void Terrain::fill_terrain( const vector<int>& linear_terrain )
{
	// 0 means unwalkable
	// 1 means walkable but unconstructable
	// 2 means constructable
	std::transform( linear_terrain.begin(),
	                linear_terrain.end(),
	                _terrain.begin(),
	                [&]( const auto& tile ){ return tile == 2 ? true : false; } );
}

bool Terrain::is_constructible( int coordinate ) const
{
	return _terrain[coordinate];
}

bool Terrain::is_constructible( int x, int y ) const
{
	return is_constructible( matrix2line( x, y ) );
}
