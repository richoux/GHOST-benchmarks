#include <ghost/global_constraints/linear_equation_leq.hpp>
#include <ghost/global_constraints/linear_equation_geq.hpp>

#include "sat_multi_knapsack_model_builder.hpp"

SatMultiKSBuilder::SatMultiKSBuilder( int number_variables,
                                int number_constraints,
                                int optimal,
                                const std::vector<std::vector<double>>& coefficients,
                                const std::vector<int>& capacities,
                                const std::vector<double>& values )
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
		constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationLeq>( variables, _capacities[i], _coefficients[i] ) );

	constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationGeq>( variables, _optimal, _values ) );
}

