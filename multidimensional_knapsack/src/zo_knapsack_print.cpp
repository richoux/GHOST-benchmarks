#include <iomanip>

#include "zo_knapsack_print.hpp"

ZeroOneKSPrint::ZeroOneKSPrint( const std::vector<int>& values )
	: _values( values )
{ }

std::stringstream ZeroOneKSPrint::print_candidate( const std::vector<ghost::Variable>& variables ) const
{
	std::stringstream ss;
	
	for( int i = 0 ; i < static_cast<int>( variables.size() ) ; ++i )
		if( variables[i].get_value() == 1 )
			ss << "Taking v[" << i
			   << "] (value = " << std::setw(2) << _values[ i ]
			   << ")\n";
	
	return ss;
}
