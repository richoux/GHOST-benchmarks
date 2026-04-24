#include <cmath>
#include "min_sum_corners.hpp"

MinCorners::MinCorners( const std::vector<ghost::Variable>& variables, int instance_size )
	: Minimize( variables, "MinSumCorners" ),
	  n(instance_size),
	  _corner_NW(0),
	  _corner_NE(n-1),
	  _corner_W(( 3*n*n - n ) / 2 - 2*n + 1 ),
	  _corner_E(( 3*n*n - n ) / 2 - 1 ),
	  _corner_SW(3*n*n - 4*n + 1),
	  _corner_SE(3*n*n - 3*n)
{ }

double MinCorners::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	return variables[_corner_NW]->get_value()
		+ variables[_corner_NE]->get_value()
		+ variables[_corner_W]->get_value()
		+ variables[_corner_E]->get_value()
		+ variables[_corner_SW]->get_value()
		+ variables[_corner_SE]->get_value();
}
