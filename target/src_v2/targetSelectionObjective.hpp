/*
 * GHOST (General meta-Heuristic Optimization Solving Tool) is a C++ library 
 * designed to help developers to model and implement optimization problem 
 * solving. 
 * GHOST is a meta-heuristic solver aiming to solve any kind of combinatorial 
 * and optimization real-time problems represented by a CSP/COP. 
 * GHOST has been first developped to help making AI for the RTS game
 * StarCraft: Brood war, but can be used for any kind of applications where 
 * solving combinatorial and optimization problems within some tenth of 
 * milliseconds is needed. It is a generalization of the Wall-in project.
 * Please visit https://github.com/richoux/GHOST for further information.
 * 
 * Copyright (C) 2014-2016 Florian Richoux
 *
 * This file is part of GHOST.
 * GHOST is free software: you can redistribute it and/or 
 * modify it under the terms of the GNU General Public License as published 
 * by the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.

 * GHOST is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.

 * You should have received a copy of the GNU General Public License
 * along with GHOST. If not, see http://www.gnu.org/licenses/.
 */


#pragma once

#include <vector>
#include <map>
#include <memory>

#include "../../src/objective.hpp"
#include "unit.hpp"
#include "unitMap.hpp"
#include "targetSelectionDomain.hpp"

using namespace std;

namespace ghost
{
  /****************************/
  /* TargetSelectionObjective */
  /****************************/
  class TargetSelectionObjective : public Objective<Unit, TargetSelectionDomain>
  {
  public:
    TargetSelectionObjective( const string &name );

  protected:
    int v_heuristicVariable( const vector< int > &vecId, const vector< Unit > *vecVariables, TargetSelectionDomain *domain );
    double v_postprocessSatisfaction( vector< Unit > *vecBuildings,
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
    double v_cost( vector< Unit > *vecVariables, TargetSelectionDomain *domain ) const;
    void v_setHelper( const Unit &u, const vector< Unit > *vecVariables, const TargetSelectionDomain *domain );
  };


  /***********/
  /* MaxKill */
  /***********/
  class MaxKill : public TargetSelectionObjective
  {
  public:
    MaxKill();
    double v_cost( vector< Unit > *vecVariables, TargetSelectionDomain *domain ) const;
    void v_setHelper( const Unit &u, const vector< Unit > *vecVariables, const TargetSelectionDomain *domain );
  };

}
