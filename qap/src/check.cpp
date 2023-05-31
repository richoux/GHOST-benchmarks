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

double check_cost( const std::vector<int>& solution,
                   int number_variables,
                   const std::vector<std::vector<int>>& distances,
                   const std::vector<std::vector<int>>& flows	)
{
	double sum = 0.0;
	
	for( int i = 0 ; i < number_variables ; ++i )
	{
		int a = solution[i] - 1;

		for( int j = 0 ; j < number_variables ; ++j )
		{
			int b = solution[j] - 1;
			sum += distances[i][j] * flows[a][b];
		}
	}

	return sum;
}
