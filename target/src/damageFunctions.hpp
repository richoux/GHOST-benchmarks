#pragma once

#include <ghost/variable.hpp>
#include <vector>

#include "unitData.hpp"

using namespace std;

vector<double> compute_damage( const UnitData& shooter, int target_index, const vector< UnitData >& units );

// vector<double> compute_my_shoot_damage( int shooter_index, const vector< ghost::Variable >& vec_variables );
// vector<double> compute_enemy_shoot_damage( int shooter_index, int target_index );
