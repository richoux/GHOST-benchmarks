#include <numeric>
#include <memory>

#include "builder_wallin.hpp"
#include "wallinConstraint.hpp"
#include "wallinObjective.hpp"
#include "convert.hpp"

BuilderWallin::BuilderWallin( std::vector<bool>& line,
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
	
	for( int i = 0 ; i < static_cast<int>( _line.size() ) ; ++i )
		if( _line[i] )
			_buildables.push_back( i );
}

void BuilderWallin::declare_variables()
{
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
