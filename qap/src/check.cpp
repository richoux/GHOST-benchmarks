#include "check.hpp"
#include "extract.hpp"

bool check_solution( const std::vector<int>& solution )
{
	std::vector<bool> check( solution.size(), false );
	
	for( auto& v : solution )
		if( check[v] )
			return false;
		else
			check[v] = true;
	
	return true;
}

double check_cost( const std::vector<int>& solution, const std::string& filename )
{
	int number_variables;
	std::vector< std::vector<int> > matrix_distances;
	std::vector< std::vector<int> > matrix_flows;

	extract_data_from_file( filename, number_variables, matrix_distances, matrix_flows );

	double sum = 0.0;
	
	for( int i = 0 ; i < number_variables ; ++i )
	{
		int a = solution[i] - 1;

		for( int j = 0 ; j < number_variables ; ++j )
		{
			int b = solution[j] - 1;
			sum += matrix_distances[i][j] * matrix_flows[a][b];
		}
	}

	return sum;
}
