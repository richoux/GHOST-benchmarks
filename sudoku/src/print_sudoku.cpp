#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

#include "print_sudoku.hpp"

void PrintSudoku::print_candidate( const std::vector<Variable>& variables ) const
{
	int size_side = static_cast<int>( std::sqrt( static_cast<int>( variables.size() ) ) );
	int size_side_small_square = static_cast<int>( std::sqrt( size_side ) );
	int space = ceil( log10( size_side ) ) + 1;
	
	std::cout << "Solution:";
	
	for( int i = 0; i < static_cast<int>( variables.size() ); ++i )
	{
		if( i%size_side == 0 )
		{
			std::cout << "\n";
			
			if( ( i/size_side) % size_side_small_square == 0 )
				for( int j = 0; j <= space*size_side + 2*( size_side_small_square - 1 ); ++j )
					std::cout << "-";
			
			std::cout << "\n";
		}
		
		if( i%size_side_small_square == 0 && i%size_side != 0)
			std::cout << std::setw(space+2) << variables[i].get_value();
		else
			std::cout << std::setw(space) << variables[i].get_value();
	}
	
	std::cout << "\n";
	
	for( int j = 0; j <= space*size_side + 2*( size_side_small_square - 1 ); ++j )
		std::cout << "-";
	
	std::cout << "\n";
}
