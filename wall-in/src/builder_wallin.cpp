#include <numeric>
#include <memory>
#include <algorithm>

#include "builder_wallin.hpp"
#include "wallinConstraint.hpp"
#include "wallinObjective.hpp"
#include "convert.hpp"

BuilderWallin::BuilderWallin( std::vector<int>& line,
                              int width,
                              int starting_tile,
                              int target_tile,
                              std::vector<Building>& buildings )
	: ModelBuilder( true ),
	  _line(line),
	  _width(width),
	  _starting_tile(starting_tile),
	  _target_tile(target_tile),
	  _buildings(buildings)
{
	// -1 means unselected building, i.e., not composing the wall.
	_buildables.push_back( -1 );
	std::copy_if( _line.begin(), _line.end(), std::back_inserter( _buildables ), [](int x) { return x == 0; });
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
