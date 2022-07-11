#include "check_solution.hpp"

bool check_solution( const std::vector<int>& solution,
                     const std::vector<std::vector<double>>& coefficients,
                     const std::vector<int>& capacities,
                     const std::vector<double>& values,
                     int optimal )
{
	bool success = true;
	
	for( int constraint_id = 0 ; constraint_id < static_cast<int>( capacities.size() ) ; ++constraint_id )
	{	
		int sum = 0;
		for( int i = 0 ; i < static_cast<int>( solution.size() ) ; ++i )
			sum += solution[i] * coefficients[constraint_id][i];
		
		if( sum > capacities[constraint_id] )
		{
			std::cout << "Multidimentional Knapsack - Error: Total weight " << sum << " is exceeding capacity " << capacities[constraint_id] << "\n";
			success = false;
		}
	}

	if( optimal != 0 )
	{
		int sum = 0;
		for( int i = 0 ; i < static_cast<int>( solution.size() ) ; ++i )
			sum += solution[i] * values[i];
		
		if( sum < optimal )
		{
			std::cout << "Multidimentional Knapsack - Error: Total value " << sum << " is less than the threshold " << optimal << "\n";
			success = false;
		}
	}

	return success;
}
