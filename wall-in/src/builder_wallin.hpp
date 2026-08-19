#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

class BuilderWallin : public ghost::ModelBuilder
{
	std::vector<int> _line; // grid where buildings are placed. -1 for unbuildable, 0 for free, n and -(n+1) for n buildings on the given (unbuildable if negative) tile
	std::vector<int> _buildables; // domain: vector of indices (to convert to coordinates)
	int _width;
	int _starting_tile;
	int _target_tile;
	std::vector<Building> _buildings;

public:
	BuilderWallin( std::vector<int>& line,
	               int width,
	               int starting_tile,
	               int target_tile,
	               std::vector<Building>& buildings );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
