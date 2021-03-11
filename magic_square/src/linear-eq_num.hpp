#pragma once

#include <vector>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

using namespace std;
using namespace ghost;

class LinearEq : public Constraint
{
	int _rhs;
	mutable int _current_diff;
	
	double required_error( const vector< Variable >& variables ) const override;

	double expert_delta_error( const vector<Variable>& variables,
	                           const vector<unsigned int>& variable_indexes,
	                           const vector<int>& candidate_values ) const override;

	void update_constraint( const vector<Variable>& variables, unsigned int variable_id, int new_value ) override;

public:
	LinearEq( const vector<Variable>& variables, int rhs );
};
