#pragma once

#include <vector>

#include <ghost/objective.hpp>
#include <ghost/variable.hpp>

#include "unitData.hpp"

///////////////
// MaxDamage //
///////////////
class MaxDamage : public ghost::Maximize
{
	const std::vector< UnitData >& _my_army;
	const std::vector< UnitData >& _enemies;
  
	double required_cost( const std::vector< ghost::Variable* >& variables ) const override;
  
public:
	MaxDamage( const std::vector<ghost::Variable>& variables,
	           const std::vector< UnitData >& my_army,
	           const std::vector< UnitData >& enemies );
};

//////////////////////
// MaxDamageMaxKill //
//////////////////////
class MaxDamageMaxKill : public ghost::Maximize
{
	const std::vector< UnitData >& _my_army;
	const std::vector< UnitData >& _enemies;
  
	double required_cost( const std::vector< ghost::Variable* >& variables ) const override;
	int expert_heuristic_value( const std::vector< ghost::Variable* >&	variables,
	                            int var_index,
	                            const std::vector< int >&	possible_values,
	                            randutils::mt19937_rng& rng	) const override;
  
public:
	MaxDamageMaxKill( const std::vector<ghost::Variable>& variables,
	                  const std::vector< UnitData >& my_army,
	                  const std::vector< UnitData >& enemies );
};

/////////////
// MaxKill //
/////////////
class MaxKill : public ghost::Maximize
{
	const std::vector< UnitData >& _my_army;
	const std::vector< UnitData >& _enemies;
  
	double required_cost( const std::vector< ghost::Variable* >& variables ) const override;
  
public:
	MaxKill( const std::vector<ghost::Variable>& variables,
	         const std::vector< UnitData >& my_army,
	         const std::vector< UnitData >& enemies );
};

/////////////////
// MinOverkill //
/////////////////
class MinOverkill : public ghost::Minimize
{
	const std::vector< UnitData >& _my_army;
	const std::vector< UnitData >& _enemies;
  
	double required_cost( const std::vector< ghost::Variable* >& variables ) const override;
  
public:
	MinOverkill( const std::vector<ghost::Variable>& variables,
	             const std::vector< UnitData >& my_army,
	             const std::vector< UnitData >& enemies );
};
