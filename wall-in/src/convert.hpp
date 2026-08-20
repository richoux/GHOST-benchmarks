#include <pair>

inline int coord_to_index( int row, int column, int width ) { return row * width + column; }
inline std::pair<int,int> index_to_coord( int index, int width ) { return make_pair<int,int>( index / width, index % width ); }
inline int index_to_row( int index, int width ) { return index / width; }
inline int index_to_column( int index, int width ) { return index % width; }
