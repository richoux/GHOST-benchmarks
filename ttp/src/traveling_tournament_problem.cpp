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
#include "convert.hpp"

#if defined TTP_OPT
#include "extract.hpp"
#endif

using namespace std::literals::chrono_literals;


int check_solution( const std::vector<int> &solution, int number_teams )
{
	int number_rounds = 2 * ( number_teams - 1 );
	int number_matches = number_teams * ( number_teams - 1 );
	std::set<int> teams;
	std::vector<int> streaks( number_teams, 0 );
	std::vector< std::vector<int> > rounds( number_rounds );
	int home = -1;
	int away = -1;

	int number_violated_constraints = 0;
	
	for( int match = 0 ; match < number_matches ; ++match )
		rounds[solution[match] - 1].push_back( match );

	for( int round = 0 ; round < number_rounds ; ++round )
	{
		teams.clear();
		for( auto& match: rounds[round] )
		{
			convert( match, number_teams, home, away );
			teams.insert( home );
			teams.insert( away );

			if( streaks[ home-1 ] <= 0 )
				streaks[ home-1 ] = 1;
			else
			{
				++streaks[ home-1 ];
				if( streaks[ home-1 ] >= 4 )
				{
					std::cout << "Error: team " << home << " has more than 3 games home (last on round " << round+1 << ")\n";
					++number_violated_constraints;
				}
			}
			
			if( streaks[ away-1 ] >= 0 )
				streaks[ away-1 ] = -1;
			else
			{
				--streaks[ away-1 ];
				if( streaks[ away-1 ] <= -4 )
				{
					std::cout << "Error: team " << away << " has more than 3 games away (last on round " << round+1 << ")\n";
					++number_violated_constraints;
				}
			}
		}

		if( teams.size() != number_teams )
		{
			std::cout << "Error: there are not " << number_teams << " team playing on round " << round+1
								<< "\nHere is the set of teams: ";
			std::copy( teams.begin(), teams.end(), std::ostream_iterator< int >(std::cout, " ") );
			std::cout << "\n";
			++number_violated_constraints;
		}
	}

	for( int match_a = 0 ; match_a < number_matches ; ++match_a )
	{
		convert( match_a, number_teams, home, away );
		if( home < away )
		{
			int match_b = match_a + ( number_teams - 2 ) * ( away - home ) + 1;
			if( std::abs( solution[ match_a ] - solution[ match_b ] ) <= 1 )
			{
				std::cout << "Error: teams " << home << " and " << away << "have two games in a row at rounds " << solution[ match_a ] << " and " << solution[ match_b ] << "\n";
				++number_violated_constraints;
			}
		}
	}

	return number_violated_constraints;
}

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
	
	ghost::Solver solver( builder );
	bool success;
  double error;
  std::vector<int> solution;
  success = solver.fast_search( error, solution, 1h, options );

  int number_violated_constraints = check_solution( solution, number_teams );
	if( number_violated_constraints > 0 )
		std::cout << "NOT A SOLUTION\n"
		          << "Number of constraints unsatisfied: " << number_violated_constraints << "\n";
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

