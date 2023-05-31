#include <fstream>
#include <sstream>

#include "tsp_builder.hpp"
#include "tsp_objective.hpp"
#include "extract.hpp"

BuilderTSP::BuilderTSP( int number_variables,
                        const std::vector< std::vector<double> >& distances )
	: ModelBuilder( true ),
	  _number_variables( number_variables ),
	  _matrix_distances( distances )
{ }

void BuilderTSP::declare_variables()
{
	// create variables
	create_n_variables( _number_variables, 1, _number_variables );

	// initialize them for the permutation
	for( int i = 0; i < _number_variables; ++i )
		variables[i].set_value( i+1 );
}

void BuilderTSP::declare_constraints()
{ }

void BuilderTSP::declare_objective()
{
	objective = std::make_shared<TSPObjective>( variables,
	                                            _number_variables,
	                                            _matrix_distances );
}
