#pragma once

#include <vector>

using namespace std;
using namespace ghost;

class IsWall : public Constraint
{
public:
	IsWall();
	
	virtual double required_error( const std::vector<Variable>& variables ) const;
	virtual double expert_delta_error( const std::vector<Variable>& variables,
	                                   const std::vector<unsigned int>& variable_indexes,
	                                   const std::vector<int>& candidate_values ) const;
};
