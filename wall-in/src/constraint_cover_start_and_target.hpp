#pragma once

#include <vector>

using namespace std;
using namespace ghost;

class CoverStartAndTarget : public Constraint
{
public:
	CoverStartAndTarget();
	
	double required_error( const vector<Variable>& variables ) const override;

  double expert_delta_error( const std::vector<Variable>& variables,
	                           const std::vector<unsigned int>& variable_indexes,
	                           const std::vector<int>& candidate_values ) const override;

	void update_constraint( const std::vector<Variable>& variables,
	                        unsigned int variable_index,
	                        int new_value );
};
