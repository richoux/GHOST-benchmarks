#pragma once

#include <vector>
#include <ghost/model_builder.hpp>

class BuilderWallin : public ghost::ModelBuilder
{
	std::vector<bool> _line;
	std::vector<int> _buildables;
	int _width;
	int _starting_tile;
	int _target_tile;
	std::vector<Building> _buildings;

public:
	BuilderWallin( std::vector<bool>& line,
	               int width,
	               int starting_tile,
	               int target_tile,
	               std::vector<Building>& buildings );

	void declare_variables() override;
	void declare_constraints() override;
	void declare_objective() override;
};
