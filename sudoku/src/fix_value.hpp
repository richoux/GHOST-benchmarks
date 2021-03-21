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

	double required_error( const vector< Variable >& variables ) const override;
	double expert_delta_error( const vector<Variable>& variables,
	                           const vector<unsigned int>& variable_indexes,
	                           const vector<int>& candidate_values ) const override;
	
	void update_constraint( const vector<Variable>& variables, unsigned int variable_id, int new_value ); 
public:
	FixValue( const vector< Variable >& variables, int value );
};
