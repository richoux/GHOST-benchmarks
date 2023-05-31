#include <fstream>
#include <sstream>

#include "qap_builder.hpp"
#include "qap_objective.hpp"
#include "extract.hpp"

BuilderQAP::BuilderQAP( int number_variables,
                        const std::vector< std::vector<int> >& matrix_distances,
                        const std::vector< std::vector<int> >& matrix_flows )
	: ModelBuilder( true ),
	  _number_variables( number_variables ),
	  _matrix_distances( matrix_distances ),
	  _matrix_flows( matrix_flows )
{ }

void BuilderQAP::declare_variables()
{
	// create variables
	create_n_variables( _number_variables, 1, _number_variables );

	// initialize them for the permutation
	for( int i = 0; i < _number_variables; ++i )
		variables[i].set_value( i+1 );
}

void BuilderQAP::declare_constraints()
{ }

void BuilderQAP::declare_objective()
{
	objective = std::make_shared<QAPObjective>( variables,
	                                            _number_variables,
	                                            _matrix_distances,
	                                            _matrix_flows );
}
