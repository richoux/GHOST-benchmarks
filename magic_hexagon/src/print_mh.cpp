#include <iostream>
#include <iomanip>
#include <string>
#include <cmath>

#include "print_mh.hpp"

PrintMagicHexagon::PrintMagicHexagon( int instance_size )
	: _instance_size(instance_size)
{ }

std::stringstream PrintMagicHexagon::print_candidate( const std::vector<ghost::Variable>& variables ) const
{
	std::stringstream stream;

	int indent = std::ceil( std::log10( variables.size() ) ) + 2;
	int index = 0;

	stream << "\n";

	for( size_t row = 0 ; row < _instance_size ; ++row )
	{
		// offset
		for( int i = 0 ; i < _instance_size - row - 1 ; ++i )
			stream << std::setw( indent ) << " ";
					
		for( int i = 0 ; i < _instance_size + row ; ++i )
		{		
			stream << std::setw( indent ) << variables[index + i].get_value();		
			stream << std::setw( indent ) << " ";		
		}

		index += _instance_size + row;
		stream << "\n";
	}

	for( size_t row = 1 ; row < _instance_size ; ++row )
	{
		// offset
		for( int i = 0 ; i < row ; ++i )
			stream << std::setw( indent ) << " ";
					
		for( int i = 0 ; i < 2*_instance_size - 1 - row ; ++i )
		{		
			stream << std::setw( indent ) << variables[index + i].get_value();		
			stream << std::setw( indent ) << " ";		
		}

		index += _instance_size + row;
		stream << "\n";
	}
	
	stream << "\n";
	return stream;
}
