#include "constraint_no_overlap.hpp"

NoOverlap::NoOverlap( const vector<Variable>& variables,
                      const vector<Building>& buildings,
                      int width,
                      int height )
	: Constraint( variables ),
	  _buildings( buildings ),
	  _placement( Placement( width, height, buildings ) ),
	  _width( width ),
	  _height( height )
{ }

double NoOverlap::required_error( const std::vector<Variable>& variables ) const
{
	double count = 0.0;
	vector<bool> visited( _width * _height, false );
	
	for( int index = 0 ; index < static_cast<int>( variables.size() ) ; ++index )
	{
		auto position = variables[ index ].get_value();
		if( position >= 0 )
		{
			for( int h = 0 ; h < _building[ index ].get_height() ; ++h )
				for( int w = 0 ; w < _building[ index ].get_width() ; ++w )
					if( !visited( position + _width*h + w ) )
					{
						count += static_cast<int>( _placement.what_is_at( position + _width*h + w ).size() ) - 1;
						visited( position + _width*h + w ) = true;
					}
		}
	}

	_error = count;
	return count;
}

// double NoOverlap::expert_delta_error( const std::vector<Variable>& variables,
//                                       const std::vector<unsigned int>& variable_indexes,
//                                       const std::vector<int>& candidate_values ) const
// {

// }

void NoOverlap::update_constraint( const std::vector<Variable>& variables,
                                   unsigned int variable_index,
                                   int new_value )
{
	auto position = variables[variable_index].get_value();
	if( position >= 0 )
	{
		for( int h = 0 ; h < _building[ variable_index ].get_height() ; ++h )
			for( int w = 0 ; w < _building[ variable_index ].get_width() ; ++w )
				if( static_cast<int>( _placement.what_is_at( position + _width*h + w ).size() ) > 1 )
					--_error;
	}
	
	_placement.remove( variable_index, position );
	_placement.add( variable_index, new_value );

	if( new_value >= 0 )
	{
		for( int h = 0 ; h < _building[ variable_index ].get_height() ; ++h )
			for( int w = 0 ; w < _building[ variable_index ].get_width() ; ++w )
				if( static_cast<int>( _placement.what_is_at( new_value + _width*h + w ).size() ) > 0 )
					++_error;
	}
}
