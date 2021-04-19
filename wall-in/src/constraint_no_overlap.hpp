#pragma once

#include <vector>
#include <ghost/constraint.hpp>

#include "grid.hpp"

using namespace std;
using namespace ghost;

class NoOverlap : public Constraint
{
	const Grid *_grid;
public:
	NoOverlap( const std::vector<Variable>& variables, const Grid& grid);
	
	double required_error( const std::vector<Variable>& variables ) const override;
	// double expert_delta_error( const std::vector<Variable>& variables,
	//                            const std::vector<unsigned int>& variable_indexes,
	//                            const std::vector<int>& candidate_values ) const override;
};
