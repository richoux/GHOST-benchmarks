#include <iostream>
#include <ghost/solver.hpp>
#include <vector>
#include <memory>
#include <string>
#include <chrono>

#include "knapsack_model_builder.hpp"
#include "knapsack_print.hpp"

using namespace std::literals::chrono_literals;

int main( int argc, char** argv )
{
	KSBuilder builder;

	std::shared_ptr<ghost::Print> printer = std::make_shared<KSPrint>( std::vector<int>{4,2,2,1,10}, std::vector<int>{12,2,1,1,4} );
	
	double cost;
	std::vector<int> solution;

	ghost::Options options;
	options.print = printer;
	options.parallel_runs = true;
	
	ghost::Solver solver( builder );
	solver.solve( cost, solution, 1s, options );	
}
