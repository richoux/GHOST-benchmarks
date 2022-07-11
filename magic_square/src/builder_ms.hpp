#pragma once

#include <ghost/model_builder.hpp>

#if defined MINMS
#include "min_sum_corners.hpp"
#endif

class BuilderMagicSquare : public ghost::ModelBuilder
{
	int _instance_size;
	int _nb_vars;
	int _constant;
	std::vector< std::vector<int> > _rows;
  std::vector< std::vector<int> > _columns;
  std::vector< std::vector<int> > _diagonals;

public:
	BuilderMagicSquare( int instance_size );
	
	void declare_variables() override;
	void declare_constraints() override;
#if defined MINMS
	void declare_objective() override;
#endif	
};
