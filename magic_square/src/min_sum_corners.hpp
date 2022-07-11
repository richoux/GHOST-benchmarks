#pragma once

#include <ghost/objective.hpp>

class MinCorners : public ghost::Minimize
{
	int _number_variables;
	int _instance_size;
	
public:
	MinCorners( const std::vector<ghost::Variable>& variables );
	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
