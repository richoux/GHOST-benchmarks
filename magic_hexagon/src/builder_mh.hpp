#pragma once

#include <ghost/model_builder.hpp>
#include "hexagon.hpp"

#if defined MINMH
#include "min_sum_corners.hpp"
#endif

class BuilderMagicHexagon : public ghost::ModelBuilder
{
	int _instance_size;
	Hexagon _hexagon;
	int _nb_vars;
	
public:
	BuilderMagicHexagon( int instance_size );
	
	void declare_variables() override;
	void declare_constraints() override;
#if defined MINMH
	void declare_objective() override;
#endif	
};
