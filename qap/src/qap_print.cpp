#include <iomanip>

#include "qap_print.hpp"

std::stringstream QAPPrint::print_candidate( const std::vector<ghost::Variable>& variables ) const
{
	std::stringstream ss;
	
	for( size_t i = 0 ; i < variables.size() ; ++i )
		ss << std::setw(6) << variables[i].get_value();

	ss << "\n";
	return ss;
}
