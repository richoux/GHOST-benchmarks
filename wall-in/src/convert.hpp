#pragma once

inline int coord_to_index( int row, int column, int width ) { return row * width + column; }
inline int index_to_row( int index, int width ) { return index / width; }
inline int index_to_column( int index, int width ) { return index % width; }
