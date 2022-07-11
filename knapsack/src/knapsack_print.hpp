#pragma once

#include <vector>
#include <ghost/print.hpp>

class KSPrint : public ghost::Print
{
	std::vector<double> _weights;
	std::vector<double> _values;
	
public:
	KSPrint( const std::vector<double>& weights, const std::vector<double>& values );
	
	std::stringstream print_candidate( const std::vector<ghost::Variable>& variables ) const override;
};	

