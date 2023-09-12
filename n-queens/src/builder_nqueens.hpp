#pragma once

#include <ghost/model_builder.hpp>

class BuilderNQueens : public ghost::ModelBuilder
{
	int _n;

public:
	BuilderNQueens( int n );

	void declare_variables() override;
	void declare_constraints() override;
};
