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

bool check_solution( const vector<int>& solution, int constant )
{
	bool success = true;
	
	if( solution[0] + solution[1] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (0,1) uncovered.\n"; success = false; }

	if( solution[0] + solution[8] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (0,8) uncovered.\n"; success = false; }

	if( solution[0] + solution[9] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (0,9) uncovered.\n"; success = false; }

	if( solution[1] + solution[2] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (1,2) uncovered.\n"; success = false; }

	if( solution[1] + solution[8] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (1,8) uncovered.\n"; success = false; }

	if( solution[2] + solution[3] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (2,3) uncovered.\n"; success = false; }

	if( solution[3] + solution[4] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (3,4) uncovered.\n"; success = false; }

	if( solution[3] + solution[5] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (3,5) uncovered.\n"; success = false; }

	if( solution[3] + solution[6] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (3,6) uncovered.\n"; success = false; }

	if( solution[3] + solution[7] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (3,7) uncovered.\n"; success = false; }

	if( solution[4] + solution[5] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (4,5) uncovered.\n"; success = false; }

	if( solution[4] + solution[6] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (4,6) uncovered.\n"; success = false; }

	if( solution[5] + solution[6] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (5,6) uncovered.\n"; success = false; }

	if( solution[5] + solution[19] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (5,19) uncovered.\n"; success = false; }

	if( solution[6] + solution[7] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (6,7) uncovered.\n"; success = false; }

	if( solution[6] + solution[15] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (6,15) uncovered.\n"; success = false; }

	if( solution[6] + solution[16] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (6,16) uncovered.\n"; success = false; }

	if( solution[7] + solution[8] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (7,8) uncovered.\n"; success = false; }

	if( solution[7] + solution[15] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (7,15) uncovered.\n"; success = false; }

	if( solution[8] + solution[9] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (8,9) uncovered.\n"; success = false; }

	if( solution[8] + solution[10] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (8,10) uncovered.\n"; success = false; }

	if( solution[9] + solution[10] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (9,10) uncovered.\n"; success = false; }

	if( solution[10] + solution[11] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (10,11) uncovered.\n"; success = false; }

	if( solution[10] + solution[14] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (10,14) uncovered.\n"; success = false; }

	if( solution[11] + solution[12] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (11,12) uncovered.\n"; success = false; }

	if( solution[12] + solution[13] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (12,13) uncovered.\n"; success = false; }

	if( solution[13] + solution[14] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (13,14) uncovered.\n"; success = false; }

	if( solution[14] + solution[15] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (14,15) uncovered.\n"; success = false; }

	if( solution[15] + solution[16] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (15,16) uncovered.\n"; success = false; }

	if( solution[16] + solution[17] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (16,17) uncovered.\n"; success = false; }

	if( solution[17] + solution[18] == 0 )
		{ std::cout << "Vertex Cover - Error: edge (17,18) uncovered.\n"; success = false; }

	if( std::accumulate( solution.begin(), solution.end(), 0) > constant )
	{
		std::cout << "Vertex Cover - Too many vertices selected: "
		          << std::accumulate( solution.begin(), solution.end(), 0)
		          << " instead of " << constant << " max.\n";
		success = false;
	}

	return success;
}

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

	bool success = check_solution( solution, 11 );
	
	if( success )
		return EXIT_SUCCESS;
	else
		return EXIT_FAILURE;
}

