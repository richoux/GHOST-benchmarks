#pragma once

#include <vector>
#include <ghost/constraint.hpp>

class MultiKSCapacity : public ghost::Constraint
{
	std::vector<int> _coefficients;
	int _capacity;

public:
	MultiKSCapacity( const std::vector<ghost::Variable>& variables, const std::vector<int>& coefficients, int capacity );

	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
};
