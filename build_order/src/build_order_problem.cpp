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
	int target_time;

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

	BuilderBO builder( input );
	ghost::Solver solver( builder );
	double error;
	std::vector<int> solution;

	solver.fast_search( error, solution, 30ms, options );


	
	// Define objective
	shared_ptr<BuildOrderObjective> objective = make_shared<MakeSpanMaxProd>( input, vec );

	// Define domain
	BuildOrderDomain domain( vec.size(), &vec );
  
	// Define constraints
	vector< shared_ptr<BuildOrderConstraint> > vecConstraints { make_shared<BuildOrderConstraint>( &vec, &domain ) };

	Solver<Action, BuildOrderDomain, BuildOrderConstraint> solver(&vec, &domain, vecConstraints, objective );

	solver.fast_search( sat, opt );
	cout << "Target_Time reference: " << static_cast<double>(target_time)/23.81 << endl;
}
