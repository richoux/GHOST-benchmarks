#pragma once

#include <vector>
#include <functional>

#include <ghost/constraint.hpp>
#include <ghost/variable.hpp>

#include "unitData.hpp"

using namespace std;
using namespace ghost;

class Shootable : public Constraint
{
  vector< UnitData > _my_army;
  vector< UnitData > _enemy_army;
  
  double required_cost() const override;
  
public:
  Shootable( const vector< reference_wrapper<Variable> >& variables,
	     const vector< UnitData >& my_army,
	     const vector< UnitData >& enemy_army );
};
