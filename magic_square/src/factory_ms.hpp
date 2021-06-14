#pragma once

#include <ghost/factory_model.hpp>

#include "linear-eq_num.hpp"

using namespace ghost;
using namespace std;

class FactoryMagicSquare : public FactoryModel
{
	int _instance_size;
	int _nb_vars;
	int _constant;
	vector< vector<int> > _rows;
  vector< vector<int> > _columns;
  vector< vector<int> > _diagonals;

public:
	FactoryMagicSquare( int instance_size );
	
	void declare_variables() override;
	void declare_constraints() override;
};
