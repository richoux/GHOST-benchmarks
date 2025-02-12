#include "check_solution.hpp"

bool check_solution( const std::vector<int>& solution, int capacity, const std::vector<double>& weights )
{
	int sum = 0;
	for( int i = 0 ; i < static_cast<int>( solution.size() ) ; ++i )
		sum += solution[i] * weights[i];

	if( sum > capacity )
	{
		std::cout << "Knapsack - Error: Total weight " << sum << " is exceeding capacity " << capacity << "\n";
		return false;
	}
	else
		return true;
}
