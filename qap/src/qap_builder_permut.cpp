#include <fstream>
#include <sstream>

#include "qap_builder.hpp"
#include "qap_objective.hpp"
#include "extract.hpp"

BuilderQAP::BuilderQAP( std::string filename )
	: ModelBuilder( true )
{
	extract_data_from_file( filename, _number_variables, _matrix_distances, _matrix_flows );
}

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
