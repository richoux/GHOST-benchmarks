#pragma once

#include <vector>
#include <functional>

#include <ghost/constraint.hpp>
#include <ghost/variable.hpp>

#include "unitData.hpp"

class Shootable : public ghost::Constraint
{
	std::vector< UnitData > _my_army;
	std::vector< UnitData > _enemies;
  
  double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
  
public:
	Shootable( const std::vector< ghost::Variable >& variables,
	           const std::vector< UnitData >& my_army,
	           const std::vector< UnitData >& enemies );
};
