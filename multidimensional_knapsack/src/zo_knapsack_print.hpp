#pragma once

#include <vector>
#include <ghost/print.hpp>

class ZeroOneKSPrint : public ghost::Print
{
	std::vector<int> _values;
	
public:
	ZeroOneKSPrint( const std::vector<int>& values );
	
	std::stringstream print_candidate( const std::vector<ghost::Variable>& variables ) const override;
};	

