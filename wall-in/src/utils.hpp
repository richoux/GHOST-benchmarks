#pragma once

#include <tuple>

std::tuple<int,int> line2matrix( int coordinate, int terrain_length );
int matrix2line( int x, int y, int terrain_length );
