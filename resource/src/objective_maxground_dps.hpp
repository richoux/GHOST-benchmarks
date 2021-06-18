#pragma once

#include <string>
#include <vector>
#include <memory>

#include <ghost/objective.hpp>
#include <ghost/variable.hpp>

#include "data.hpp"

using namespace std;
using namespace ghost;

class MaxGroundDPS : public Objective
{
  vector<UnitData> _unit_data;
  
  double required_cost( const vector<Variable*>& vecVariables ) const override;

public:
	MaxGroundDPS( const vector<Variable>& variables, shared_ptr<AuxiliaryData> auxiliary_data );
};
