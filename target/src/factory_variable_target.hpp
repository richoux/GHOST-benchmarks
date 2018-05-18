#pragma once

#include <ghost/variable.hpp>

#include "unitData.hpp"
#include "unitType.hpp"

using namespace ghost;
using namespace std;

UnitData make_data( UnitType, Coord );
Variable make_unit( UnitType, int );

void make_my_terran( vector<Variable>&, vector<UnitData>& );
void make_enemy_terran( vector<UnitData>& );
