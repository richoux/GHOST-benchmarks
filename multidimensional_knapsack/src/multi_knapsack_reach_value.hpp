#pragma once

#include <vector>
#include <ghost/constraint.hpp>

class MultiKSReachValue : public ghost::Constraint
{
	std::vector<int> _values;
	int _optimal;
	mutable double _cache_sum_values;

public:
	MultiKSReachValue( const std::vector<ghost::Variable>& variables, const std::vector<int>& values, int optimal );

	double required_error( const std::vector<ghost::Variable*>& variables ) const override;
	double optional_delta_error( const std::vector<ghost::Variable*>& variables, const std::vector<int>& indexes, const std::vector<int>& candidate_values ) const override;
	void conditional_update_data_structures( const std::vector<ghost::Variable*>& variables, int variable_index, int new_value ) override;
};
