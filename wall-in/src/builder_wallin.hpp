#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

#include "building.hpp"

class BuilderWallin : public ghost::ModelBuilder
{
	std::vector<std::vector<bool>> _grid;
	int _width;
	int _height;
	int _starting_tile;
	int _target_tile;
	std::vector<int> _buildables; // domain: vector of indices (to convert to coordinates)
	std::vector<Building> _buildings;

public:
	BuilderWallin( const std::vector<std::vector<bool>>& grid,
	               int starting_tile,
	               int target_tile,
	               const std::vector<Building>& buildings );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
