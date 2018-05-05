#pragma once

#include <string>
#include <vector>

#include <ghost/objective.hpp>
#include <ghost/variable.hpp>

#include "unitData.hpp"

using namespace std;
using namespace ghost;

class MaxGroundDPS : public Objective
{
  vector< UnitData > _unit_data;
  
  double required_cost( const vector< Variable > &vecVariables ) const override;

public:
  MaxGroundDPS( const vector< UnitData >& unit_data );
};
