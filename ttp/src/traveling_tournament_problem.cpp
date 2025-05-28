#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>
#include <ghost/solver.hpp>

#include "builder_ttp.hpp"
#include "print_ttp.hpp"
#include "convert.hpp"

#if defined TTP_OPT
#include "extract.hpp"
#endif

using namespace std::literals::chrono_literals;

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
		std::cout << "Usage: " << argv[0] << " N [parallel=0/1] [number_threads]\n";
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
	
	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );

#if defined TTP_OPT
	BuilderTTP builder( number_teams, distances );
#else
	BuilderTTP builder( number_teams );
#endif
	
  // true means it is a permutation problem
	ghost::Solver solver( builder );
	bool success;
  double error;
  std::vector<int> solution;
  success = solver.fast_search( error, solution, 1min, options );

	int home = -1;
	int away = -1;
	int count = 1;
	int indent = std::ceil( std::log10( number_teams ) ) + 1;

	for( size_t match = 0 ; match < solution.size() ; ++match )
	{
		convert( match, number_teams, home, away );
		if( home != away )
			std::cout << std::setw( indent ) << home << "/" << away << std::setw( indent+1 ) << solution[match] <<", ";

		if( count % ( number_teams - 1 ) == 0 )
			std::cout << "\n";
		++count;			
	}
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

