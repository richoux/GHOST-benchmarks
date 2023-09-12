#pragma once

#include <vector>

#include <ghost/constraint.hpp>
#include <ghost/variable.hpp>

using namespace ghost;

class Diagonal : public Constraint
{
	std::vector<int> _positions;
	
	double required_error( const std::vector<Variable*>& variables ) const override;

public:
	Diagonal( const std::vector<int>& variables_index,
            const std::vector<int>& positions );	
};
