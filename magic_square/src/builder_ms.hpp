#pragma once

#include <ghost/model_builder.hpp>

#include "linear-eq_num.hpp"
#if defined MINMS
#include "min_sum_corners.hpp"
#endif

using namespace ghost;
using namespace std;

class BuilderMagicSquare : public ModelBuilder
{
	int _instance_size;
	int _nb_vars;
	int _constant;
	vector< vector<int> > _rows;
  vector< vector<int> > _columns;
  vector< vector<int> > _diagonals;

public:
	BuilderMagicSquare( int instance_size );
	
	void declare_variables() override;
	void declare_constraints() override;
#if defined MINMS
	void declare_objective() override;
#endif	
};
