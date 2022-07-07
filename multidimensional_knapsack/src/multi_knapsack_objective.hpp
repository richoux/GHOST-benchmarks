#pragma once

#include <vector>
#include <ghost/objective.hpp>

class MultiKSObjective : public ghost::Maximize
{
	std::vector<int> _values;

public:
	MultiKSObjective( const std::vector<ghost::Variable>& variables, const std::vector<int>& values );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
