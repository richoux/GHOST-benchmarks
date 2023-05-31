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
	std::vector< std::vector<double> > matrix_distances;

	extract_data_from_file( filename, number_variables, matrix_distances );

	double tour = 0.;

	for( int i = 0 ; i < number_variables - 1 ; ++i )
	{
		int a = solution[i] - 1;
		int b = solution[i+1] - 1;
		tour += matrix_distances[a][b];
	}

	int a = solution[number_variables-1] - 1;
	int b = solution[0] - 1;
	tour += matrix_distances[a][b];
	
	return tour;
}
