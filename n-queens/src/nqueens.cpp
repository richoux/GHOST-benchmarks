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

std::stringstream print_solution( const std::vector<int>& solution )
{
	std::stringstream stream;
	int n = static_cast<int>( solution.size() );
	std::string line = "-";

	std::vector<int> queen_by_row( solution.size() );
	for( int i = 0 ; i < n ; ++i )
	{
		int row = solution[ i ];
		queen_by_row[ row ] = i;
		line = line + "--";
	}
	
	stream << "Solution:\n";
	std::string queen_or_empty;
	
	for( int row = 0; row < n; ++row )
	{
		stream << line << "\n";
		for( int col = 0; col < n; ++col )
		{
			if( queen_by_row[ row ] == col )
				queen_or_empty = "♛";
			else
				queen_or_empty = " ";
			stream << "|" << queen_or_empty;
		}
		stream << "|\n";
	}
	stream << line << "\n";

	return stream;
}


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

  ghost::Options options;
	options.print = printer;
	
#if defined COMPLETESEARCH
	std::vector<double> errors;
  std::vector< std::vector<int> > solutions;
  bool success = solver.complete_search( errors, solutions, options );
  if( success )
	  for( int i = 0 ; i < static_cast<int>( solutions.size() ) ; ++i )
	  {
		  std::cout << "Solution " << i << ": cost=" << errors[i] << "\n"
		            << print_solution( solutions[i] ).str() << "\n";
	  }  
#else
  double error;
  std::vector<int> solution;
  bool success = solver.fast_search( error, solution, 1s, options );
  if( success )
	  std::cout << print_solution( solution ).str() << "\n";
#endif
  
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;  
}

