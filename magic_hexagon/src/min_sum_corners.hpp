#pragma once

#include <ghost/objective.hpp>

class MinCorners : public ghost::Minimize
{
	int n; // instance size
	
	int _corner_NW;
	int _corner_NE;
	int _corner_W;
	int _corner_E;
	int _corner_SW;
	int _corner_SE;
	
public:
	MinCorners( const std::vector<ghost::Variable>& variables, int instance_size );
	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
