#pragma once

#include <vector>
#include <ghost/print.hpp>

class ZeroOneKSPrint : public ghost::Print
{
	std::vector<double> _values;
	
public:
	ZeroOneKSPrint( const std::vector<double>& values );
	
	std::stringstream print_candidate( const std::vector<ghost::Variable>& variables ) const override;
};	

