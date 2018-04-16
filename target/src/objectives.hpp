#pragma once

#include <vector>
#include <map>
#include <memory>

#include "ghost/objective.hpp"
#include "unit.hpp"
#include "unitMap.hpp"

using namespace std;

/****************************/
/* TargetSelectionObjective */
/****************************/
class TargetSelectionObjective : public Objective<Unit>
{
public:
  TargetSelectionObjective( const string &name );

protected:
  double expert_postprocess_satisfaction( vector< Unit > *vecBuildings,
					  TargetSelectionDomain *domain,
					  double &bestCost,
					  vector< Unit > &bestSolution,
					  double sat_timeout ) const;
};
  
  
/*************/
/* MaxDamage */
/*************/
class MaxDamage : public TargetSelectionObjective
{
public:
  MaxDamage();
  double required_cost( vector< Unit > *vecVariables, TargetSelectionDomain *domain ) const;
};


/***********/
/* MaxKill */
/***********/
class MaxKill : public TargetSelectionObjective
{
public:
  MaxKill();
  double required_cost( vector< Unit > *vecVariables, TargetSelectionDomain *domain ) const;
};


