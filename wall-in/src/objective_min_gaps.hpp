#pragma once

class MinGaps : public Objective
{
public:
	MinGaps();

	virtual double required_cost( const std::vector<Variable>& variables ) const;
};
