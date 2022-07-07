#include "check_solution.hpp"

bool check_solution( const std::vector<int>& solution, int capacity, std::vector<int>&& weights )
{
	int sum = 0;
	bool success = true;
	for( int i = 0 ; i < static_cast<int>( solution.size() ) ; ++i )
		sum += solution[i] * weights[i];

	if( sum > capacity )
	{
		std::cout << "AllDiff Knapsack - Error: Total weight " << sum << " is exceeding capacity " << capacity << "\n";
		success = false;
	}

	for( int i = 0 ; i < static_cast<int>( solution.size() ) - 1 ; ++i )
		for( int j = i+1 ; j < static_cast<int>( solution.size() ) ; ++j )
			if( solution[i] == solution[j] )
			{
				std::cout << "AllDiff Knapsack - Error: var[" << i << "] = var[" << j << "] (both at " << solution[i] << ")\n";
				success = false;
			}

	return success;
}
