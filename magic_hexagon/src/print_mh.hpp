#pragma once

#include <sstream>
#include <vector>

#include "ghost/print.hpp"
#include "ghost/variable.hpp"

class PrintMagicHexagon : public ghost::Print
{
	int _instance_size;
	
public:
	PrintMagicHexagon( int );
	
	std::stringstream print_candidate( const std::vector<ghost::Variable>& variables ) const override;
};
