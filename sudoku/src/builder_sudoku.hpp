#pragma once

#include <ghost/model_builder.hpp>

class BuilderSudoku : public ghost::ModelBuilder
{
	int _instance_size;
	int _side_size;
	bool _hard_instance;
	std::vector< std::vector<int> > _rows;
  std::vector< std::vector<int> > _columns;
  std::vector< std::vector<int> > _squares;

public:
	BuilderSudoku( int instance_size,
	               bool hard_instance );

	void declare_variables() override;
	void declare_constraints() override;
};
