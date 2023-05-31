#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

#include <ghost/solver.hpp>

#include "qap_builder.hpp"
#include "qap_print.hpp"
#include "check.hpp"
#include "extract.hpp"

using namespace std::literals::chrono_literals;

void usage( char* name )
{
	std::cout << "Usage: " << name << " INSTANCE_FILE [parallel=1] [NUMBER_CORES]\n";
}

///////////////////////

int main( int argc, char **argv )
{
	bool parallel = false;
	int cores = -1;
	
	if( argc < 2 || argc > 4 )
	{
		usage(argv[0]);
		return EXIT_FAILURE;
	}

	std::string filename( argv[1] );
	if( argc >= 3 )
		parallel = ( std::stoi( argv[2] ) != 0 );
	if( argc == 4 && parallel )
		cores = std::stoi( argv[3] );

  BuilderQAP builder( filename );
  ghost::Solver solver( builder );

  double cost;
  std::vector<int> solution;

  std::shared_ptr<ghost::Print> printer = std::make_shared<QAPPrint>();
  ghost::Options options;
  options.print = printer;
	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );
	
  solver.solve( cost, solution, 5s, options );		

  bool success = check_solution( solution );
	
	if( success )
	{
		std::cout << "Best cost found: " << cost
		          << "\nChecking cost: " << check_cost(solution, filename) << "\n";

		std::string filename_without_extension = filename.substr(0, filename.find_last_of(".") );
		std::string solutionfile = filename_without_extension + ".sln";

		std::vector<int> optimal;
		int optimal_cost;

		extract_solution_from_file( solutionfile, optimal_cost, optimal );
		
		std::cout << "Optimal or best known solution: ";
		std::copy(optimal.begin(), optimal.end(), std::ostream_iterator<int>( std::cout, " ") );
		std::cout << "\nOptimal or best known cost: " << optimal_cost << "\n";
		
		return EXIT_SUCCESS;
	}
	else
		return EXIT_FAILURE;
}
