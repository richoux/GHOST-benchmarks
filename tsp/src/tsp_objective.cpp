#include "tsp_objective.hpp"

TSPObjective::TSPObjective( const std::vector<ghost::Variable>& variables,
                            int number_variables,
                            const std::vector< std::vector<double> >& matrix_distances )
	: Minimize( variables, "TSP" ),
	  _number_variables( number_variables ),
	  _matrix_distances( matrix_distances )
{ }

double TSPObjective::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double tour = 0.;

	for( int i = 0 ; i < _number_variables - 1 ; ++i )
	{
		int a = variables[i]->get_value() - 1;
		int b = variables[i+1]->get_value() - 1;
		tour += _matrix_distances[a][b];
	}

	int a = variables[_number_variables-1]->get_value() - 1;
	int b = variables[0]->get_value() - 1;
	tour += _matrix_distances[a][b];

	return tour;
}
