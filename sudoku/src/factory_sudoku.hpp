#pragma once

#include <ghost/factory_model.hpp>

#include "all-diff.hpp"
#include "fix_value.hpp"

using namespace ghost;
using namespace std;

class FactorySudoku : public FactoryModel
{
	int _instance_size;
	int _side_size;
	bool _hard_instance;
	vector< vector<int> > _rows;
  vector< vector<int> > _columns;
  vector< vector<int> > _squares;

public:
	FactorySudoku( int instance_size,
	               bool hard_instance );

	void declare_variables() override;
	void declare_constraints() override;
};
