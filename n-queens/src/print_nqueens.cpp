#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

#include "print_nqueens.hpp"

std::stringstream PrintNQueens::print_candidate( const std::vector<ghost::Variable>& variables ) const
{
	std::stringstream stream;
	int n = static_cast<int>( variables.size() );
	std::string line = "-";

	for( int i = 0 ; i < n ; ++i )
		line = line + "--";
	
	stream << "Solution:\n";
	std::string queen_or_empty;
	
	for( int row = 0; row < n; ++row )
	{
		stream << line << "\n";
		for( int col = 0; col < n; ++col )
		{
			if( variables[ row ].get_value() == col )
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
