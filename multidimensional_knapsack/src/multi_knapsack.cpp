#include <iostream>
#include <iomanip>
#include <sstream>
#include <fstream>
#include <string>

#include <ghost/solver.hpp>
#include <vector>
#include <memory>
#include <chrono>

#include "multi_knapsack_model_builder.hpp"
#include "sat_multi_knapsack_model_builder.hpp"
#include "zo_knapsack_print.hpp"
#include "check_solution.hpp"

using namespace std::literals::chrono_literals;

int main( int argc, char** argv )
{
	std::string filepath, line;
	std::ifstream file;
	std::string string_number;

	int number_variables;
	int number_constraints;
	int optimal;
	int integer;
	
	if( argc == 1 )
	{
		std::cout << "Usage: " << argv[0] << " FILE\n";
		return EXIT_FAILURE;
	}
	else
	{
		filepath = argv[1];
	}

	file.open( filepath );
	getline( file, line );
	std::stringstream line_stream( line );

	// First line
	line_stream >> number_variables >> number_constraints >> optimal;

	std::vector<std::vector<int>> coefficients( number_constraints, std::vector<int>( number_variables ) );
	std::vector<int> capacities( number_constraints );
	std::vector<int> values( number_variables );

	getline( file, line );
	line_stream = std::stringstream( line );
	// Values of each variable
	for( int var_index = 0 ; var_index < number_variables ; ++var_index )
	{
		line_stream >> string_number;
		std::stringstream number_stream( string_number );
		number_stream >> integer;
		values[var_index] = integer;
	}

	// Coefficients matrix [number_constraints x number_variables]
	for( int constraint_id = 0 ; constraint_id < number_constraints ; ++constraint_id )
	{
		getline( file, line );
		std::stringstream line_stream( line );
		for( int var_index = 0 ; var_index < number_variables ; ++var_index )
		{
			line_stream >> string_number;
			std::stringstream number_stream( string_number );
			number_stream >> integer;
			coefficients[constraint_id][var_index] = integer;
		}
	}

	getline( file, line );
	line_stream = std::stringstream( line );
	// Capacities
	for( int constraint_id = 0 ; constraint_id < number_constraints ; ++constraint_id )
	{
		line_stream >> string_number;
		std::stringstream number_stream( string_number );
		number_stream >> integer;
		capacities[constraint_id] = integer;
	}

	std::shared_ptr<ghost::Print> printer = std::make_shared<ZeroOneKSPrint>( values );
	
	double cost;
	std::vector<int> solution;

	ghost::Options options;
	options.print = printer;
	options.parallel_runs = true;
	//options.number_threads = 16;
	
	if( optimal == 0 )
	{
		MultiKSBuilder builder( number_variables, number_constraints, coefficients, capacities, values );
#if defined GHOST_BENCH or defined GHOST_TRACE
		std::cout << "OPTIMIZATION version of the multidimensional knapsack problem.\n";
#endif
		
		ghost::Solver solver( builder );
		solver.solve( cost, solution, 1min, options );
	}
	else
	{
		optimal -= static_cast<int>( optimal / 100 ); // relax the optimality to 1%
		
		SatMultiKSBuilder builder( number_variables, number_constraints, optimal, coefficients, capacities, values );
#if defined GHOST_BENCH or defined GHOST_TRACE
		std::cout << "SATISFACTION version of the multidimensional knapsack problem.\n";
#endif

		ghost::Solver solver( builder );
		solver.solve( cost, solution, 1min, options );
	}

	bool success = check_solution( solution, coefficients, capacities, values, optimal );

	if( success )
			return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}
