#include "utils.hpp"

std::tuple<int,int> line2matrix( int coordinate, int terrain_length )
{
	// return make_pair<int,int>( coordinate % terrain_length, coordinate / terrain_length );
	return { coordinate % terrain_length, coordinate / terrain_length };
}

int matrix2line( int x, int y, int terrain_length )
{
	return y * terrain_length + x;
}

// int matrix2line( std::pair<int,int> coordinates, int terrain_length )
// {
// 	return matrix2line( coordinates.first, coordinates.second, terrain_length );
// }

