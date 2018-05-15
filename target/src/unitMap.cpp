#include "unitMap.hpp"

map<string, const UnitData> unitOf
{
  make_pair( "Terran_Marine",
	     UnitData( "Terran_Marine", 40, 0, Small, 0, 15, 6, Normal, {0, 128}, {0,0,0}, false ) ),
    make_pair( "Terran_Firebat",
  	       UnitData( "Terran_Firebat", 50, 1, Small, 0, 22, 16, Concussive, {0, 32}, {15,20,25}, true, true ) ),
    make_pair( "Terran_Ghost",
  	       UnitData( "Terran_Ghost", 45, 0, Small, 0, 22, 10, Concussive, {0, 244}, {0,0,0}, false ) ),
    make_pair( "Terran_Vulture",
  	       UnitData( "Terran_Vulture", 80, 0, Medium, 0, 30, 20, Concussive, {0, 160}, {0,0,0}, false ) ),
    make_pair( "Terran_Goliath",
  	       UnitData( "Terran_Goliath", 125, 1, Large, 0, 22, 12, Normal, {0, 192}, {0,0,0}, false ) ),
    make_pair( "Terran_Siege_Tank_Tank_Mode",
  	       UnitData( "Terran_Siege_Tank_Tank_Mode", 150, 1, Large, 0, 37, 30, Explosive, {0, 224}, {0,0,0}, false ) ),
    make_pair( "Terran_Siege_Tank_Siege_Mode",
  	       UnitData( "Terran_Siege_Tank_Siege_Mode", 150, 1, Large, 0, 75, 70, Explosive, {64, 384}, {10,25,40}, true ) )
    };

