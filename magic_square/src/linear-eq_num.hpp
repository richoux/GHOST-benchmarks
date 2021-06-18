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
	
	double required_error( const vector<Variable*>& variables ) const override;

	double expert_delta_error( const vector<Variable*>& variables,
	                           const vector<int>& variable_indexes,
	                           const vector<int>& candidate_values ) const override;

	void expert_update_if_delta_error_defined( const vector<Variable*>& variables, int variable_id, int new_value ) override;

public:
	LinearEq( const vector<int>& index, int rhs );
};
