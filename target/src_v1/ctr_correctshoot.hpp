#pragma once

#include <vector>
#include <iostream>
#include <memory>
#include <algorithm>
#include <string>

#include "ghost/constraint.hpp"
#include "var_unit.hpp"
#include "unitMap.hpp"

using namespace std;

namespace ghost
{
  class TargetSelectionConstraint : public Constraint<Unit>
  {
  public:
    TargetSelectionConstraint( vector< Unit >* );

    vector<UnitEnemy> getEnemiesInRange( const Unit& );
    vector<UnitEnemy> getLivingEnemiesInRange( const Unit& );

  private:
    double required_cost () const override;
  };  
}
