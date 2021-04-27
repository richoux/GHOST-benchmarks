#include <algorithm>

#include "placement.hpp"

Placement::Placement( int width, int height, const vector< Building >& buildings )
	: Grid( width, height ),
	  _placement( vector< vector<int> >( width * height) ),
	  _building( building )
{ }	

Placement::Placement( const ifstream& file_data )
{ }

void Placement::fill( const vector<Variable>& variables )
{
	vector<int> positions( variables.size() );
	
	std::transform( variables.begin(),
	                variables.end(),
	                positions.begin(),
	                [&]( const auto& var ){ return var.get_value(); } );

	fill( positions );
}

void Placement::fill( const vector<int>& positions )
{
	for( int id = 0 ; id < static_cast<int>( positions.size() ) ; ++id )
		if( positions[id] >= 0 )
			add( id, positions[id] );
}

void Placement::add( int building_id, int position )
{
	for( int h = 0 ; h < _building[ building_id ].get_height() ; ++h )
		for( int w = 0 ; w < _building[ building_id ].get_width() ; ++w )
			_placement[ position + width*h + w ].push_back( building_id );
}

// return false iff the building is not placed at 'position'
bool Placement::remove( int building_id, int position )
{
	for( int h = 0 ; h < _building[ building_id ].get_height() ; ++h )
		for( int w = 0 ; w < _building[ building_id ].get_width() ; ++w )
		{
			auto iterator = std::find( _placement[ position + width*h + w ].begin(),
			                           _placement[ position + width*h + w ].end(),
			                           building_id );
			
			if( iterator == _placement[ position + width*h + w ].end() )
				return false;

			_placement[ position + width*h + w ].erase( iterator );
		}
	
	return true;
}

vector<int> Placement::what_is_at( int coordinate ) const
{
	return _placement[ coordinate ];
}

vector<int> Placement::what_is_at( int x, int y ) const
{
	return what_is_at( matrix2line( x, y ) );
}
