#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

#include <ghost/solver.hpp>

#include "builder_nqueens.hpp"
#include "print_nqueens.hpp"

using namespace std::literals::chrono_literals;

int main( int argc, char **argv )
{
	int n;

	if( argc == 1 )
		n = 4;
	else
		n = std::stoi( argv[1] );

	std::shared_ptr<ghost::Print> printer = std::make_shared<PrintNQueens>();

  BuilderNQueens builder( n );
  ghost::Solver solver( builder );

  double error;
  std::vector<int> solution;

  ghost::Options options;
	options.print = printer;
	
  bool success = solver.solve( error, solution, 1s, options );		
  // bool success = check_solution( solution );
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;  
}

