#pragma once

#include <string>
#include <vector>

#include <ghost/variable.hpp>

#include "unitData.hpp"

using namespace std; 
using namespace ghost; 

enum UnitType{ Zealot,
	       Dragoon,
	       DarkTemplar,
	       Reaver,
	       Scout,
	       Marine,
	       Firebat,
	       Ghost,
	       Vulture,
	       SiegeTankTankMode,
	       SiegeTankSiegeMode,
	       Goliath,
	       Wraith,
	       BattleCruiser,
	       Zergling,
	       Hydralisk,
	       Lurker,
	       Ultralisk,
	       Mutalisk,
	       Guardian };

Variable make_unit( const UnitType&, int, UnitData& );

void make_protoss( int, vector<Variable>&, vector<UnitData>& );
void make_terran( int, vector<Variable>&, vector<UnitData>& );
void make_zerg( int, vector<Variable>&, vector<UnitData>& );
