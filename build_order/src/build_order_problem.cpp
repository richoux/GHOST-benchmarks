#include <fstream>
#include <iostream>
#include <vector>
#include <memory>
#include <algorithm>
#include <type_traits>
#include <string>

#include <ghost/options.hpp>
#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>
#include <ghost/objective.hpp>
#include <ghost/solver.hpp>

#include "builder_bo.hpp"

using namespace std::literals::chrono_literals;

int main(int argc, char **argv)
{
	bool parallel = false;
	int cores = -1;

	if( argc < 2 || argc > 4 )
	{
		std::cout << "Usage: " << argv[0] << " INPUTFILE [parallel=0/1] [number_threads]\n";
		return EXIT_FAILURE;
	}
	else
	{
		if( argc >= 3 )
			parallel = ( std::stoi( argv[2] ) != 0 );
		if( argc == 4 && parallel )
			cores = std::stoi( argv[3] );
	}
		
  ghost::Options options;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );
	
	// BO list
	std::vector< std::pair<std::string, int> > input;
  
	std::ifstream inputFile;
	std::ofstream outputFile;
	char read[256];
	std::string str;
	int count = 0;

	std::string action;
	int target_time = 0;

	std::vector<std::string> variables_name;
	
	inputFile.open( argv[1], std::ifstream::in );
	if( inputFile.is_open() && inputFile.peek() != std::ifstream::traits_type::eof() )
	{
		while( inputFile >> read )
		{
			str = read;

			if( count % 3 == 0 )
			{
				if( isdigit( str[0] ) )
				{
					target_time = stoi(str);
					break;
				}
				else
					action = str;
			}
			else if( count % 3 == 1 )
			{
				input.emplace_back( action, stoi(str) );
			}

			++count;
		}
	}

	for( const auto& action: input )
		for( int i = 0 ; i < action.second ; ++i )
			variables_name.push_back( action.first );
	
	BuilderBO builder( input );
	ghost::Solver solver( builder );
	double cost;
	std::vector<int> solution;

	solver.fast_search( cost, solution, 30ms, options );

	std::vector<std::string> bo( solution.size() );
	for( size_t i = 0 ; i < solution.size() ; ++i )
		bo[ solution[ i ] ] = variables_name[i];
	
	std::cout << "BO:\n";
	for( size_t i = 0 ; i < bo.size() ; ++i )
		std::cout << std::setw(2) << i+1 << ". " << bo[i] << "\n";
	std::cout << "\nSolution Time: " << static_cast<double>(cost)/23.81 << std::endl
	          << "Target_Time reference: " << static_cast<double>(target_time)/23.81 << std::endl;
}
