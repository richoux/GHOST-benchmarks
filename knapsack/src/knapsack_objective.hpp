#pragma once

#include <vector>
#include <memory>
#include <ghost/objective.hpp>

class KSObjective : public ghost::Maximize
{
	std::vector<double> _values;

public:
	KSObjective( const std::vector<ghost::Variable>& variables, const std::vector<double>& values );

	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
