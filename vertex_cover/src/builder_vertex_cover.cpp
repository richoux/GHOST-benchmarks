#include <numeric>

#include "builder_vertex_cover.hpp"

BuilderVC::BuilderVC()
	: ModelBuilder(),
	  _constant( 11 ),
	  _all_variables( 20 )
{
	std::iota( _all_variables.begin(), _all_variables.end(), 0 );
}

void BuilderVC::declare_variables()
{
	create_n_variables( 20, 0, 2 );
}

void BuilderVC::declare_constraints()
{
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{0,1}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{0,8}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{0,9}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{1,2}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{1,8}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{2,3}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,7}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,6}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,5}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,4}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{4,6}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{4,5}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{5,6}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{5,19}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{6,16}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{6,15}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{6,7}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{7,15}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{7,8}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{8,10}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{8,9}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{9,10}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{10,11}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{10,14}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{11,12}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{12,13}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{13,14}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{14,15}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{15,16}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{16,17}, 1 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{17,18}, 1 ) );

	constraints.emplace_back( make_shared< LinearLeq >( _all_variables, _constant ) );
}
