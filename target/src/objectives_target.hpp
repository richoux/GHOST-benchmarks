#pragma once

#include <vector>

#include <ghost/objective.hpp>
#include <ghost/variable.hpp>

#include "unitData.hpp"

using namespace std;
using namespace ghost;

///////////////
// MaxDamage //
///////////////
class MaxDamage : public Objective
{
  const vector< UnitData >& _my_army;
  const vector< UnitData >& _enemy_army;
  
  double required_cost( const vector< Variable >& vec_variables ) const override;
  
public:
  MaxDamage( const vector< UnitData >& my_army,
	     const vector< UnitData >& enemy_army );
};

/////////////
// MaxKill //
/////////////
class MaxKill : public Objective
{
  const vector< UnitData >& _my_army;
  const vector< UnitData >& _enemy_army;
  
  double required_cost( const vector< Variable >& vec_variables ) const override;
  
public:
  MaxKill( const vector< UnitData >& my_army,
	   const vector< UnitData >& enemy_army );
};

/////////////////
// MinOverkill //
/////////////////
class MinOverkill : public Objective
{
  const vector< UnitData >& _my_army;
  const vector< UnitData >& _enemy_army;
  
  double required_cost( const vector< Variable >& vec_variables ) const override;
  
public:
  MinOverkill( const vector< UnitData >& my_army,
	       const vector< UnitData >& enemy_army );
};
