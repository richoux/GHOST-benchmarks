#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

#include "print_ms.hpp"

stringstream PrintMagicSquare::print_candidate( const std::vector<Variable>& variables ) const
{
	stringstream stream;

	int nb_vars = static_cast<int>( variables.size() );
	int order = static_cast<int>( std::sqrt( nb_vars ) );
	int indent = ceil( log10( nb_vars ) ) + 1;
	
	for( int i = 0 ; i < nb_vars ; ++i )
	{
		if( i%order == 0 )
			stream << "\n";
		
		stream << setw( indent ) << variables[i].get_value();
	}
	
	stream << "\n";
	return stream;
}
