#pragma once

#include "unitData.hpp"
#include "unitType.hpp"

UnitData make_data( UnitType, Coord );

void make_my_terran( std::vector<UnitType>&, std::vector<UnitData>& );
void make_enemy_terran( std::vector<UnitData>& );
