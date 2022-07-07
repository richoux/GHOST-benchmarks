#include "sat_multi_knapsack_model_builder.hpp"
#include "multi_knapsack_capacity.hpp"
#include "multi_knapsack_min_value.hpp"

SatMultiKSBuilder::SatMultiKSBuilder( int number_variables,
                                int number_constraints,
                                int optimal,
                                const std::vector<std::vector<int>>& coefficients,
                                const std::vector<int>& capacities,
                                const std::vector<int>& values )
	: ModelBuilder(),
	  _number_variables( number_variables ),
	  _number_constraints( number_constraints ),
	  _optimal( optimal ),
	  _coefficients( coefficients ),
	  _capacities( capacities ),
	  _values( values )	  
{ }

void SatMultiKSBuilder::declare_variables()
{
	create_n_variables( _number_variables, 0, 2 ); // 0-1 knapsack
}

void SatMultiKSBuilder::declare_constraints()
{
	for( int i = 0 ; i < _number_constraints ; ++i )
		constraints.emplace_back( std::make_shared<MultiKSCapacity>( variables, _coefficients[i], _capacities[i] ) );

	constraints.emplace_back( std::make_shared<MultiKSMinValue>( variables, _values, _optimal ) );
}

