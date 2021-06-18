#pragma once

#include <vector>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

using namespace std;
using namespace ghost;

class AllDiff : public Constraint
{
	mutable vector<int> _count;
	
	double required_error( const vector<Variable*>& variables ) const override;
	
	double expert_delta_error( const vector<Variable*>& variables,
	                           const vector<int>& variable_indexes,
	                           const vector<int>& candidate_values ) const override;
	
	void expert_update_if_delta_error_defined( const vector<Variable*>& variables,
	                                           int variable_index,
	                                           int new_value ) override;

public:
	AllDiff( const vector<int>& variables_index );
};
