#include <iostream>
#include <fstream>
#include <string>

#include <vector>
#include <algorithm>
#include <cmath>
#include <chrono>

#include <ghost/solver.hpp>

#include "builder_vertex_cover.hpp"
#include "print_vertex_cover.hpp"

using namespace ghost;
using namespace std;
using namespace std::literals::chrono_literals;

int main( int argc, char **argv )
{
	bool parallel = false;
	int cores = -1;
	
	if( argc >= 2 )
			parallel = ( std::stoi( argv[1] ) != 0 );
	if( argc == 3 && parallel )
		cores = std::stoi( argv[2] );
	
  shared_ptr<Print> printer = make_shared<PrintVertexCover>();

  BuilderVC builder;
  Solver solver( builder );

  double error;
  vector<int> solution;

	Options options;
	options.print = printer;

	if( parallel )
		options.parallel_runs = true;
	
	if( cores != -1 )
		options.number_threads = static_cast<unsigned int>( cores );
	
  solver.solve( error, solution, 5s, options );		

	cout << "Error: " << error << "\n";
	
  return EXIT_SUCCESS;
}

