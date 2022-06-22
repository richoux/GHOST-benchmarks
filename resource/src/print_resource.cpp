#include <string>

#include "print_resource.hpp"

std::stringstream PrintResource::print_candidate( const std::vector<Variable>& variables ) const
{
	std::stringstream stream;
	
	for( const auto& v : variables )
		stream << v.get_name() << ": " << v.get_value() << "\n";

	return stream;
}
