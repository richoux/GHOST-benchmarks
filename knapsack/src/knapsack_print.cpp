#include <iomanip>

#include "knapsack_print.hpp"

KSPrint::KSPrint( const std::vector<double>& weights, const std::vector<double>& values )
	: _weights( std::move( weights ) ),
	  _values( std::move( values ) )
{ }

std::stringstream KSPrint::print_candidate( const std::vector<ghost::Variable>& variables ) const
{
	std::stringstream ss;
	
	for( size_t i = 0 ; i < variables.size() ; ++i )
		ss << "Number of v[" << i
		   << "] (value = " << std::setw(2) << _values[ i ]
		   << ", weight = " << std::setw(2) << _weights[ i ]
		   << "): " << std::setw(2) << variables[i].get_value() << "\n";

	return ss;
}
