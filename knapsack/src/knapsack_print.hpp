#pragma once

#include <vector>
#include <ghost/print.hpp>

class KSPrint : public ghost::Print
{
	std::vector<int> _values;
	std::vector<int> _weights;
	
public:
	KSPrint( std::vector<int>&& values, std::vector<int>&& weights );
	
	std::stringstream print_candidate( const std::vector<ghost::Variable>& variables ) const override;
};	

