#include <ghost/global_constraints/linear_equation_leq.hpp>

#include "knapsack_model_builder.hpp"
#include "knapsack_objective.hpp"

KSBuilder::KSBuilder( const std::vector<double>& weights,
                      const std::vector<double>& values )
	: ModelBuilder(),
	  _weights( weights ),
	  _values( values )
{ }

void KSBuilder::declare_variables()
{
	create_n_variables( 5, 0, 16 );
}

void KSBuilder::declare_constraints()
{
	constraints.emplace_back( std::make_shared<ghost::global_constraints::LinearEquationLeq>( variables, 15.0, _weights ) ); // knapsack capacity
}

void KSBuilder::declare_objective()
{
	objective = std::make_shared<KSObjective>( variables, _values );
}
