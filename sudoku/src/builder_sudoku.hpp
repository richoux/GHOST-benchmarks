#pragma once

#include <ghost/model_builder.hpp>

#include "all-diff.hpp"
#include "fix_value.hpp"

using namespace ghost;
using namespace std;

class BuilderSudoku : public ModelBuilder
{
	int _instance_size;
	int _side_size;
	bool _hard_instance;
	vector< vector<int> > _rows;
  vector< vector<int> > _columns;
  vector< vector<int> > _squares;

public:
	BuilderSudoku( int instance_size,
	               bool hard_instance );

	void declare_variables() override;
	void declare_constraints() override;
};
