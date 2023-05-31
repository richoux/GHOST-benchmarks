#include "qap_objective.hpp"

QAPObjective::QAPObjective( const std::vector<ghost::Variable>& variables,
                            int number_variables,
                            const std::vector< std::vector<int> >& matrix_distances,
                            const std::vector< std::vector<int> >& matrix_flows )
	: Minimize( variables, "Min quadratic product" ),
	  _number_variables( number_variables ),
	  _matrix_distances( matrix_distances ),
	  _matrix_flows( matrix_flows )
{ }

double QAPObjective::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double sum = 0.0;
	
	for( int i = 0 ; i < _number_variables ; ++i )
	{
		int a = variables[i]->get_value() - 1;

		for( int j = 0 ; j < _number_variables ; ++j )
		{
			int b = variables[j]->get_value() - 1;
			sum += _matrix_distances[i][j] * _matrix_flows[a][b];
		}
	}
	return sum;
}
