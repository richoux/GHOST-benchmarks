#include <iostream>
#include <ghost/solver.hpp>
#include <vector>
#include <memory>
#include <string>
#include <chrono>

#include "knapsack_model_builder.hpp"
#include "knapsack_print.hpp"
#include "check_solution.hpp"

using namespace std::literals::chrono_literals;

int main( int argc, char** argv )
{
	std::vector<double> weights{12,2,1,1,4};
	std::vector<double> values{4,2,2,1,10};
	
	KSBuilder builder( weights, values );

	std::shared_ptr<ghost::Print> printer = std::make_shared<KSPrint>( weights, values );
	
	double cost;
	std::vector<int> solution;

	ghost::Options options;
	options.print = printer;
	
	ghost::Solver solver( builder );
	solver.solve( cost, solution, 100ms, options );

	bool success = check_solution( solution, 15, weights );
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}
