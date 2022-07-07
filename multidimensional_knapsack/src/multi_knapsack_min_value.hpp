#pragma once

#include <vector>
#include <ghost/constraint.hpp>

class MultiKSMinValue : public ghost::Constraint
{
	std::vector<int> _values;
	int _optimal;

public:
	MultiKSMinValue( const std::vector<ghost::Variable>& variables, const std::vector<int>& values, int optimal );

	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
};
