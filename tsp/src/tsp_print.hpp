#pragma once

#include <vector>
#include <ghost/print.hpp>

class TSPPrint : public ghost::Print
{
public:
	std::stringstream print_candidate( const std::vector<ghost::Variable>& variables ) const override;
};	

