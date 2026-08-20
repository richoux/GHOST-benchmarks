#include <numeric>
#include <memory>
#include <algorithm>

#include "builder_wallin.hpp"
#include "wallinConstraint.hpp"
#include "wallinObjective.hpp"
#include "convert.hpp"

BuilderWallin::BuilderWallin( const std::vector<std::vector<bool>>& grid,                              
                              int starting_tile,
                              int target_tile,
                              const std::vector<Building>& buildings )
	: ModelBuilder(),
	  _grid(grid),
	  _width(grid[0].size()),
	  _height(grid.size()),
	  _starting_tile(starting_tile),
	  _target_tile(target_tile),
	  _buildings(buildings)
{
	// -1 means unselected building, i.e., not composing the wall.
	_buildables.push_back( -1 );

	for( int r = 0; r < _height; ++r )
		for( int c = 0; c < _width; ++c )
			if( grid[r][c] )
				_buildables.push_back( coord_to_index( r, c, _width ) );
}

void BuilderWallin::declare_variables()
{
	// we could also filter out all values that would imply recovering an unbuildable tile.
	create_variables( _buildings.size(), _buildables );
}

void BuilderWallin::declare_constraints()
{
	constraints.emplace_back( std::make_shared<NoRepeat>( pair ) );
}

void BuilderWallin::declare_objective()
{
	objective = make_shared<MinSpan>( variables, _exhaustive_inputs );
}
