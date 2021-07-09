#include <iomanip>

#include "knapsack_print.hpp"

KSPrint::KSPrint( std::vector<int>&& values, std::vector<int>&& weights )
	: _values( std::move( values ) ),
	  _weights( std::move( weights ) )
{ }

std::stringstream KSPrint::print_candidate( const std::vector<ghost::Variable>& variables ) const
{
	std::stringstream ss;
	
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		ss << "Number of v[" << i
		   << "] (value = " << std::setw(2) << _values[ i ]
		   << ", weight = " << std::setw(2) << _weights[ i ]
		   << "): " << std::setw(2) << variables[i].get_value() << "\n";

	return ss;
}
