#pragma once

class MinLength : public Objective
{
public:
	MinLength();

	virtual double required_cost( const std::vector<Variable>& variables ) const;
};
