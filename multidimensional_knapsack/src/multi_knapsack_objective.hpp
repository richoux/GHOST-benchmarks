#pragma once

#include <vector>
#include <ghost/objective.hpp>

class MultiKSObjective : public ghost::Maximize
{
	std::vector<double> _values;

public:
	MultiKSObjective( const std::vector<ghost::Variable>& variables, const std::vector<double>& values );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
