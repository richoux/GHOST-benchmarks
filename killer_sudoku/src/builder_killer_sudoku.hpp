#pragma once

#include <ghost/model_builder.hpp>

class BuilderKillerSudoku : public ghost::ModelBuilder
{
	int _instance_size;
	int _side_size;
	int _nb_vars;
	std::vector< std::vector<int> > _rows;
  std::vector< std::vector<int> > _columns;
  std::vector< std::vector<int> > _squares;
  std::vector< std::vector<int> > _cages;

public:
	BuilderKillerSudoku();

	void declare_variables() override;
	void declare_constraints() override;
};
