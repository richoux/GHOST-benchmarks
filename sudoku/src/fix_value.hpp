#pragma once

#include <vector>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

using namespace std;
using namespace ghost;

class FixValue : public Constraint
{
	int _value;
	mutable int _current_diff;

	double required_error( const vector<Variable*>& variables ) const override;
	double expert_delta_error( const vector<Variable*>& variables,
	                           const vector<int>& variable_indexes,
	                           const vector<int>& candidate_values ) const override;
	
public:
	FixValue( const vector<int>& variables_index, int value );
};
