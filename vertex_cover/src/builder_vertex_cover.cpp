#include <numeric>

#include "builder_vertex_cover.hpp"

BuilderVC::BuilderVC()
	: ModelBuilder(),
	  _constant( (11*2)+9 ), // 91 or 11 selected
	  _all_variables( 20 )
{
	std::iota( _all_variables.begin(), _all_variables.end(), 0 );
}

void BuilderVC::declare_variables()
{
	create_n_variables( 20, 1, 2 );
}

void BuilderVC::declare_constraints()
{
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{0,1}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{0,8}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{0,9}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{1,2}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{1,8}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{2,3}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,7}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,6}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,5}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{3,4}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{4,6}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{4,5}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{5,6}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{5,19}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{6,16}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{6,15}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{6,7}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{7,15}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{7,8}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{8,10}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{8,9}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{9,10}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{10,11}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{10,14}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{11,12}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{12,13}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{13,14}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{14,15}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{15,16}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{16,17}, 2, 3 ) );
	constraints.emplace_back( make_shared< LinearGeq >( std::vector<int>{17,18}, 2, 3 ) );

	constraints.emplace_back( make_shared< LinearLeq >( _all_variables, 2, _constant ) );
}
