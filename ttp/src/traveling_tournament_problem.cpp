#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <set>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ghost/solver.hpp>

#include "builder_ttp.hpp"
#include "print_ttp.hpp"
#include "misc.hpp"

using namespace std::literals::chrono_literals;

/////////////////////////

int main( int argc, char **argv )
{
	int number_teams;
	bool parallel = false;
	int cores = -1;

#if defined TTP_OPT
	std::vector< std::vector<double> > distances;
#endif
	
	if( argc == 1 )
	{
#if defined TTP_OPT
		std::cout << "Usage: " << argv[0] << " FILE [parallel=0/1] [number_threads]\n";
#else
		std::cout << "Usage: " << argv[0] << " N [parallel=0/1] [number_threads]\n";
#endif
		return EXIT_FAILURE;
	}
	else
	{
#if defined TTP_OPT
		std::string filename( argv[1] );
		extract_data_from_file( filename, number_teams, distances );
#else
		number_teams = std::stoi( argv[1] );
#endif
		if( argc >= 3 )
			parallel = ( std::stoi( argv[2] ) != 0 );
		if( argc == 4 && parallel )
			cores = std::stoi( argv[3] );
	}
		
	std::shared_ptr<ghost::Print> printer = std::make_shared<PrintTTP>();
  ghost::Options options;
	options.print = printer;
	options.number_start_samplings = number_teams * number_teams;
	// options.enable_optimization_guidance = false;
	
	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );

#if defined TTP_OPT
	BuilderTTP builder( number_teams, distances );
#else
	BuilderTTP builder( number_teams );
#endif
	
	ghost::Solver solver( builder );
	bool success;
  double error;
  std::vector<int> solution;
  success = solver.fast_search( error, solution, 1s, options );

  int number_violated_constraints = check_error_solution( solution, number_teams );
	if( number_violated_constraints > 0 )
		std::cout << "NOT A SOLUTION\n"
		          << "Number of constraints unsatisfied: " << number_violated_constraints << "\n";

#if defined TTP_OPT
	std::cout << "Cost check: " << check_cost_solution( solution, number_teams, distances ) << "\n";
#endif
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

