#include <fstream>
#include <sstream>

#include <ghost/global_constraints/all_different.hpp>

#include "tsp_builder.hpp"
#include "tsp_objective.hpp"
#include "extract.hpp"

BuilderTSP::BuilderTSP( int number_variables,
                        const std::vector< std::vector<double> >& distances )
	: ModelBuilder(),
	  _number_variables( number_variables ),
	  _matrix_distances( distances )
{ }

void BuilderTSP::declare_variables()
{
	// create variables
	create_n_variables( _number_variables, 1, _number_variables );
}

void BuilderTSP::declare_constraints()
{
	constraints.emplace_back( std::make_shared<ghost::global_constraints::AllDifferent>( variables ) );
}

void BuilderTSP::declare_objective()
{
	objective = std::make_shared<TSPObjective>( variables,
	                                            _number_variables,
	                                            _matrix_distances );
}
