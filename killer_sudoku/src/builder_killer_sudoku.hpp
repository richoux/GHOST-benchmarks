#pragma once

#include <ghost/model_builder.hpp>

#include "all-diff.hpp"
#include "linear-eq.hpp"

using namespace ghost;
using namespace std;

class BuilderKillerSudoku : public ModelBuilder
{
	int _instance_size;
	int _side_size;
	int _nb_vars;
	vector< vector<int> > _rows;
  vector< vector<int> > _columns;
  vector< vector<int> > _squares;
  vector< vector<int> > _cages;

public:
	BuilderKillerSudoku();

	void declare_variables() override;
	void declare_constraints() override;
};
