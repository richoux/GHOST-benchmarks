#pragma once

#include <vector>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

using namespace std;
using namespace ghost;

class LinearEq : public Constraint
{
	int _rhs;
	mutable int _current_sum;
	mutable int _higher_rhs; // -1 means _rhs < _current_sum, 0 means _rhs = _current_sum, 1 means _rhs > _current_sum
	mutable vector<Variable> _copy_variables;
	
	double required_error( const vector< Variable >& variables ) const override;

	double expert_delta_error( const vector<Variable>& variables,
	                           const vector<unsigned int>& variable_indexes,
	                           const vector<int>& candidate_values ) const override;

	void update_constraint( const vector<Variable>& variables, unsigned int variable_id, int new_value ) override;

public:
	LinearEq( const vector<Variable>& variables, int rhs );
};
