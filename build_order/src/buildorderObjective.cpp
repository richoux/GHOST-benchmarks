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


#include <vector>
#include <map>
#include <memory>
#include <algorithm>
#include <limits>
#include <cmath>
#include <chrono>
#include <ctime>
#include <numeric>
#include <iomanip>

#include "buildorderObjective.hpp"
#include "action.hpp"
#include "actionMap.hpp"

using namespace std;

namespace ghost
{
  /***********************/
  /* BuildOrderObjective */
  /***********************/
  BuildOrderObjective::BuildOrderObjective( const string &name )
    : Objective<Action, BuildOrderDomain>( name, true ),
    currentState( State() ),
    goals( map<string, pair<int, int> >() ),
    bo( vector<BO>() )
  { }
  
  BuildOrderObjective::BuildOrderObjective( const string &name,
					    const vector< pair<string, int> > &input,
					    vector<Action> &variables )
    : Objective<Action, BuildOrderDomain>( name, true ),
    currentState( State() ),
    goals( map<string, pair<int, int> >() ),
    bo( vector<BO>() )
  {
    for( const auto &i : input)
      makeVecVariables( i, variables, goals );
  }

  void BuildOrderObjective::printBO() const
  {
    cout << endl << endl;
    for( const auto &b : bestBO )
    {
      if( b.fullName.compare("Protoss_Probe") != 0 && b.fullName.compare("Protoss_Pylon") != 0 )
	cout << b.fullName
	     << ": start at " << b.startTime
	     << ", finish at " << b.completedTime << endl;
    }
    cout << endl;
  }
  
  double BuildOrderObjective::v_cost( vector< Action > *vecVariables, BuildOrderDomain *domain ) const
  {
    currentState.reset();
    bo.clear();
    for( auto &g : goals)
      g.second.second = 0;

    auto actionToDo = vecVariables->begin();
    
    while( actionToDo != vecVariables->end() || !currentState.busy.empty() )
    {
      // cout << "vec size: " << vecVariables->size()
      // 	   << ", iter: " << std::distance( actionToDo, vecVariables->end() )
      // 	   << ", busy: " << currentState.busy.size() << endl;

      // if( currentState.seconds > 700 )
      // {
      // 	for( auto itit = actionToDo ; itit != vecVariables->end() ; ++itit )
      // 	  cout << itit->getFullName() << "(" << canHandleNotBuilding( *itit ) << ") ";
      // 	cout << endl;	
      // }
      
	++currentState.seconds;

      // update mineral / gas stocks
      currentState.stockMineral += currentState.mineralWorkers * minRate; // minRate mineral per worker per second in average
      currentState.stockGas += currentState.gasWorkers * gasRate; // gasRate gas per worker per second in average

      // update busy list
      updateBusy();

      // update inMove list
      updateInMove();

      if( actionToDo != vecVariables->end() )
      {
	dealWithWorkers();
	
	// build a pylon if I must, ie:
	// 1. if I am not currently making pylons
	// 2. if my supply cap cannot manage the next global unit production
	if( !makingPylons() || currentState.numberPylons != 0 )
	  youMustConstructAdditionalPylons();

	// can I produce units?
	if( actionToDo->getType() != ActionType::unit )
	  produceUnitsFirst( actionToDo, vecVariables );
	
	// can I handle the current action?
	if( handleActionToDo( *actionToDo ) )
	  ++actionToDo;
	else // can I handle the next action?
	{
	  auto nextAction = actionToDo + 1;
	  if( nextAction != vecVariables->end() )
	  {
	    // book resources for the current action
	    int mineralCost = actionToDo->getCostMineral();
	    int gasCost = actionToDo->getCostGas();
	    
	    currentState.mineralsBooked += mineralCost;
	    currentState.gasBooked += gasCost;
	    if ( canHandleBuilding( *nextAction ) || canHandleNotBuilding( *nextAction ) )
	    {
	      // cout << "Swap " << actionToDo->getFullName() << ":" << actionToDo->getValue()
	      // 	   << " with " << nextAction->getFullName() << ":" << nextAction->getValue() << endl;

	      std::swap( *actionToDo, *nextAction );
	      actionToDo->swapValue( *nextAction );
	      currentState.mineralsBooked -= mineralCost;
	      currentState.gasBooked -= gasCost;
	      if( handleActionToDo( *actionToDo ) )
		++actionToDo;
	      else
	      {
		cout << "This should never append." << endl;
		exit(0);
	      }	      
	    }
	    else
	    {
	      currentState.mineralsBooked -= mineralCost;
	      currentState.gasBooked -= gasCost;
	    }
	  }
	}
      }
    }
    return static_cast<double>( currentState.seconds );
  }

  double BuildOrderObjective::costOpti( vector< Action > *vecVariables ) const
  {
    currentState.reset();
    bo.clear();
    for( auto &g : goals)
      g.second.second = 0;

    vector< Action > copyVec = *vecVariables;
    
    auto actionToDo = copyVec.begin();

    while( actionToDo != copyVec.end() || !currentState.busy.empty() )
    {
      ++currentState.seconds;

      // update mineral / gas stocks
      currentState.stockMineral += currentState.mineralWorkers * minRate;
      currentState.stockGas += currentState.gasWorkers * gasRate;

      // update busy list
      updateBusy();

      // update inMove list
      updateInMove();

      if( actionToDo != copyVec.end() )
      {
	dealWithWorkers();
	
	// only used by postprocessingOptimization, to see if we can
	// shorten the makespan by making more production buildings,
	// like gateways for instance.
	ActionData action, creator;
	double real_time, simulated_time;
	int simulated_mineral, simulated_gas;
	int future_mineral, future_gas;
	  
	int to_produce;
	int creator_in_production;
	int totalNumber;
	for( const auto &g : goals )
	{
	  action = actionOf[g.first];
	  if( action.actionType == ActionType::building )
	    continue;

	  if( action.name.compare("Protoss_Archon") == 0 || action.name.compare("Protoss_Dark_Archon") == 0 )
	    continue;
	    
	  creator = actionOf[ action.creator ];

	  creator_in_production =
	    count_if( begin(currentState.inMove),
		      end(currentState.inMove),
		      [&creator](ActionPrep &t){return t.action.name.compare( creator.name ) == 0;})
	    + count_if( begin(currentState.busy),
			end(currentState.busy),
			[&creator](ActionData &a){return a.name.compare( creator.name ) == 0;});

	  totalNumber = currentState.resources[creator.name].first + creator_in_production;
	    
	  if( totalNumber == 0 )
	    continue;

	  // test is we are faster after making an additional production building
	  to_produce = g.second.first - g.second.second;
	  real_time = 0.;
	    
	  for( const auto &t : currentState.busy )
	    if( t.name.compare( action.name ) == 0 )
	    {
	      --to_produce;
	      real_time += t.secondsRequired;
	    }

	  if( to_produce <= 0 )
	    continue;

	  simulated_time = real_time + creator.secondsRequired;
	  real_time += to_produce * action.secondsRequired / ( currentState.resources[creator.name].first + creator_in_production );
	  simulated_time += to_produce * action.secondsRequired / ( currentState.resources[creator.name].first + creator_in_production + 1 );

	  if( simulated_time > real_time )
	    continue;

	  // test is we have enough money for making an additional production building
	  simulated_mineral = ( currentState.resources[creator.name].first + creator_in_production + 1 ) * action.costMineral;
	  simulated_gas = ( currentState.resources[creator.name].first + creator_in_production + 1 ) * action.costGas;

	  future_mineral = sharpMineralsIn( action.secondsRequired, creator.secondsRequired );
	  future_gas = sharpGasIn( action.secondsRequired, creator.secondsRequired );

	  // if we can make this additional building, do it! 
	  if( future_mineral >= simulated_mineral && future_gas >= simulated_gas
	      &&
	      ( creator.costMineral == 0 || currentState.stockMineral >= creator.costMineral + currentState.mineralsBooked - mineralsIn(goToBuild) )
	      &&
	      ( creator.costGas == 0 || currentState.stockGas >= creator.costGas + currentState.gasBooked - gasIn(goToBuild) ) 
	      &&
	      currentState.mineralWorkers + currentState.gasWorkers > 0
	      &&
	      currentState.numberPylons > 0
	      )
	  {
	    currentState.mineralsBooked += creator.costMineral;
	    currentState.gasBooked += creator.costGas;
	    
	    auto it_find = std::find( vecVariables->begin(), vecVariables->end(), *actionToDo );
	    auto it = vecVariables->insert( it_find, Action( creator, it_find->getValue() ) );
	    std::for_each( it+1, vecVariables->end(), [](Action &a){a.shiftValue();} );
	      
	    currentState.inMove.push_back( ActionPrep( creator, goToBuild, currentState.inMove.size() ) );
	    if( currentState.mineralWorkers > 0 )
	      --currentState.mineralWorkers;
	    else
	      --currentState.gasWorkers;

#ifndef NDEBUG
	    string text = "Optimize " + creator.name + " at ";
	    cout << std::left << setw(35) << text << setw(5) << currentState.seconds
	    	 << "  m = " << setw(9) << currentState.stockMineral
	    	 << "  g = " << setw(8) << currentState.stockGas
	    	 << "  mb = " << setw(5) << currentState.mineralsBooked
	    	 << "  gb = " << setw(4) << currentState.gasBooked
	    	 << "  mw = " << setw(3) << currentState.mineralWorkers
	    	 << "  gw = " << setw(3) << currentState.gasWorkers
		 << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif	    
	  }	    
	}	  
	
	// build a pylon if I must, ie:
	// 1. if I am not currently making pylons
	// 2. if my supply cap cannot manage the next global unit production
	if( !makingPylons() || currentState.numberPylons != 0 )
	  youMustConstructAdditionalPylons();

	// can I produce units?
	if( actionToDo->getType() != ActionType::unit )
	  produceUnitsFirst( actionToDo, &copyVec );

	// can I handle the current action?
	if( handleActionToDo( *actionToDo ) )
	  ++actionToDo;
	else // can I handle the next action?
	{
	  auto nextAction = actionToDo + 1;
	  if( nextAction != copyVec.end() )
	  {
	    // book resources for the current action
	    int mineralCost = actionToDo->getCostMineral();
	    int gasCost = actionToDo->getCostGas();
	    
	    currentState.mineralsBooked += mineralCost;
	    currentState.gasBooked += gasCost;
	    if ( canHandleBuilding( *nextAction ) || canHandleNotBuilding( *nextAction ) )
	    {
	      // cout << "Swap " << actionToDo->getFullName() << ":" << actionToDo->getValue()
	      // 	   << " with " << nextAction->getFullName() << ":" << nextAction->getValue() << endl;

	      std::swap( *actionToDo, *nextAction );
	      actionToDo->swapValue( *nextAction );
	      currentState.mineralsBooked -= mineralCost;
	      currentState.gasBooked -= gasCost;
	      if( handleActionToDo( *actionToDo ) )
		++actionToDo;
	      else
	      {
		cout << "This should never append." << endl;
		exit(0);
	      }	      
	    }
	    else
	    {
	      currentState.mineralsBooked -= mineralCost;
	      currentState.gasBooked -= gasCost;
	    }
	  }
	}
      }
    }
    return static_cast<double>( currentState.seconds );
  }

  void BuildOrderObjective::updateBusy() const
  {
    for( auto &t : currentState.busy )
    {
      int time = t.decreaseSeconds();
      if( time == 0 )
      {
	if( t.creator.compare("Protoss_Probe") != 0 )
	{
	  ++currentState.resources[ t.creator ].second;
	}
	if( t.name.compare("Protoss_Probe") == 0 )
	  currentState.inMove.push_back( ActionPrep( actionOf["Protoss_Mineral"], fromBaseToMinerals, currentState.inMove.size() ) );
	else
	{
	  if( t.name.compare("Protoss_Nexus") == 0 )
	  {
	    ++currentState.resources["Protoss_Nexus"].first;
	    ++currentState.resources["Protoss_Nexus"].second;
	    currentState.supplyCapacity += 9;
	    ++currentState.numberBases;
	  }
	  else if( t.name.compare("Protoss_Pylon") == 0 )
	  {
	    currentState.supplyCapacity += 8;
	    ++currentState.numberPylons;
	  }
	  else if( t.name.compare("Protoss_Assimilator") == 0 )
	  {
	    ++currentState.numberRefineries;
	    
	    // if we have few workers mining, do not sent them to gas
	    for( int i = 0 ; i < min( 3, currentState.mineralWorkers - 3 ) ; ++i )
	    {
	      currentState.inMove.push_back( ActionPrep( actionOf["Protoss_Gas"], fromMinToGas, currentState.inMove.size() ) );
	      --currentState.mineralWorkers;
	    }
	  }
	  else if( t.actionType == ActionType::building )
	  {
	    ++currentState.resources[ t.name ].first;
	    ++currentState.resources[ t.name ].second;
	    currentState.inMove.push_back( ActionPrep( actionOf["Protoss_Mineral"], returnToMinerals, currentState.inMove.size() ) );
	  }
	  else if( t.name.compare("Protoss_High_Templar") == 0
		   || t.name.compare("Protoss_Dark_Templar") == 0 )
	  {
	    ++currentState.resources[ t.name ].first;
	    ++currentState.resources[ t.name ].second;
	  }
	}

#ifndef NDEBUG
	string text = "Finish " + t.name + " at ";
	cout << std::left << setw(35) << text << setw(5) << currentState.seconds
	     << "  m = " << setw(9) << currentState.stockMineral
	     << "  g = " << setw(8) << currentState.stockGas
	     << "  mb = " << setw(5) << currentState.mineralsBooked
	     << "  gb = " << setw(4) << currentState.gasBooked
	     << "  mw = " << setw(3) << currentState.mineralWorkers
	     << "  gw = " << setw(3) << currentState.gasWorkers
	     << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif
      }
    }

    auto itEnd = remove_if( begin( currentState.busy ), end( currentState.busy ), [](ActionData &a){return a.secondsRequired == 0;} );
    currentState.busy.erase( itEnd, end( currentState.busy ) );
  }
  
  void BuildOrderObjective::updateInMove() const
  {
    // remove from the inMove list actions just done
    // auto itEnd = remove_if( begin( currentState.inMove ), end( currentState.inMove ), [](ActionPrep &t){return t.done;} );
    // currentState.inMove.erase( itEnd, end( currentState.inMove ) );

    // The code below is not groovy
    // but it is written that way to avoid a really weird bug
    for_each( begin( currentState.inMove ), end( currentState.inMove ), [](ActionPrep &a){ if(a.waitTime > 0) --a.waitTime; } );
    auto copyInMove(currentState.inMove);
    
    for( int i = 0 ; i < copyInMove.size() ; ++i )
    {
      // if( copyInMove[i].waitTime > 0 )
      // 	--copyInMove[i].waitTime;

      // if( t.action.name.compare("Protoss_Templar_Archives") == 0 )
      // {
      // 	cout << "Details Templar Archives: sec req=" << t.action.secondsRequired << ", wait=" << t.waitTime << ", done=" << t.done << endl;
      // }
      
      // if( t.action.name.compare("Protoss_Robotics_Support_Bay") == 0 )
      // {
      // 	cout << "Details Robo Bay: sec req=" << t.action.secondsRequired << ", wait=" << t.waitTime << ", done=" << t.done << endl;
      // }
      
      if( copyInMove[i].waitTime == 0
	  && ( copyInMove[i].action.costMineral == 0 || currentState.stockMineral >= copyInMove[i].action.costMineral )
	  && ( copyInMove[i].action.costGas == 0 || currentState.stockGas >= copyInMove[i].action.costGas )
	)
      {
	string creator = copyInMove[i].action.creator;
	string goal = copyInMove[i].action.name;

	int mineralCost = copyInMove[i].action.costMineral;
	int gasCost = copyInMove[i].action.costGas;

	if( creator.compare("Protoss_Probe") == 0 )
	{
	  // The action is about to be done, so erase it from inMove
	  auto toErase = std::find_if( begin( currentState.inMove ), end( currentState.inMove ),
				       [&](ActionPrep &a){ return copyInMove[i].id == a.id; });
	  for_each( toErase, currentState.inMove.end(), [](ActionPrep &a){ --a.id; } );
	  for_each( begin( copyInMove ) + i, end( copyInMove ), [](ActionPrep &a){ --a.id; } );
	  currentState.inMove.erase( toErase );
	  
	  if( goal.compare("Mineral") == 0 ) 
	    ++currentState.mineralWorkers;
	  else if( goal.compare("Gas") == 0 ) 
	    ++currentState.gasWorkers;
	  else // ie, the worker is about to build something
	  {
	    pushInBusy( copyInMove[i].action.name );
	    // warp building and return to mineral fields
	    currentState.inMove.push_back( ActionPrep( actionOf["Protoss_Mineral"], returnToMinerals, currentState.inMove.size() ) );
	    
	    currentState.stockMineral -= mineralCost;
	    currentState.stockGas -= gasCost;
	    
	    currentState.mineralsBooked -= mineralCost;
	    currentState.gasBooked -= gasCost;
	    
#ifndef NDEBUG
	    string text = "Start " + goal + " at ";
	    cout << std::left << setw(35) << text << setw(5) << currentState.seconds
		 << "  m = " << setw(9) << currentState.stockMineral
		 << "  g = " << setw(8) << currentState.stockGas
		 << "  mb = " << setw(5) << currentState.mineralsBooked
		 << "  gb = " << setw(4) << currentState.gasBooked
		 << "  mw = " << setw(3) << currentState.mineralWorkers
		 << "  gw = " << setw(3) << currentState.gasWorkers
		 << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif
	  } 
	}
      }
    }    
  }

  void BuildOrderObjective::dealWithWorkers() const
  {
    // send workers to gas, if need and possible
    if( currentState.gasWorkers + count_if( begin(currentState.inMove),
					    end(currentState.inMove),
					    [](ActionPrep &t){return t.action.name.compare("Gas") == 0;} )
	< currentState.numberRefineries * 3 )
    {
      // if we have few workers mining, do not sent them to gas
      for( int i = 0 ; i < min( 3, currentState.mineralWorkers - 3 ) ; ++i )
      {
	currentState.inMove.push_back( ActionPrep( actionOf["Protoss_Gas"], fromMinToGas, currentState.inMove.size() ) );
	--currentState.mineralWorkers;
      }
    }
    
    // produce a worker if I can, ie:
    // 1. if I have at least 50 minerals
    // 2. if I have at least one available Nexus
    // 3. if I am not supply blocked
    // 4. if I don't reach the saturation number (ie 24 workers per base)
    if( currentState.stockMineral >= 50
	&&
	currentState.resources["Protoss_Nexus"].second > 0
	&&
	currentState.supplyUsed < currentState.supplyCapacity
	&&
	currentState.mineralWorkers + count_if( begin(currentState.inMove),
						end(currentState.inMove),
						[](const ActionPrep &t){return t.action.creator.compare("Protoss_Probe") == 0;})
	< currentState.numberBases * 24 )
    {
      currentState.stockMineral -= 50;
      ++currentState.supplyUsed;
      --currentState.resources["Protoss_Nexus"].second;
      pushInBusy( "Protoss_Probe" );
      
#ifndef NDEBUG
      cout << std::left << setw(35) << "Start Protoss_Probe at " << setw(5) << currentState.seconds
      	   << "  m = " << setw(9) << currentState.stockMineral
      	   << "  g = " << setw(8) << currentState.stockGas
      	   << "  mb = " << setw(5) << currentState.mineralsBooked
      	   << "  gb = " << setw(4) << currentState.gasBooked
      	   << "  mw = " << setw(3) << currentState.mineralWorkers
      	   << "  gw = " << setw(3) << currentState.gasWorkers
	   << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif
    }
  }

  void BuildOrderObjective::produceUnitsFirst( vector<Action>::iterator &actionToDo, vector< Action > *vecVariables ) const
  {
    if( actionToDo == vecVariables->end() || actionToDo == vecVariables->end() - 1 )
      return;
    
    for( auto it = actionToDo + 1 ; it != vecVariables->end() ; ++it )
    {
      if( it->getType() == ActionType::unit && canHandleNotBuilding( *it ) )
      {
	for( auto it_swap = it - 1 ; it_swap != vecVariables->begin() && it_swap != actionToDo - 1 ; --it_swap )
	{
	  auto next = it_swap+1;
	  std::swap( *it_swap, *next );
	  it_swap->swapValue( *next );
	}
	
	if( handleActionToDo( *actionToDo ) )
	  ++actionToDo;
	else
	{
	  cout << "This should never append too." << endl;
	  exit(0);
	}	
      }
    }
  }
  
  bool BuildOrderObjective::canHandleBuilding( const Action &actionToDo ) const
  {
    if( actionToDo.getType() != ActionType::building )
      return false;

    // special case for the Assimilator
    // if( actionToDo.getFullName().compare("Protoss_Assimilator") == 0 )
    // {
      
    //   if(currentState.seconds > 700)
    //   {
    //   	bool plop = find_if( begin(currentState.busy),
    //   			     end(currentState.busy),
    //   			     [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} ) != currentState.busy.end();
	
    //   	cout << "nb ref: " << currentState.numberRefineries
    //   	     << ", nb base:" << currentState.numberBases
    //   	     << ", under build: " << plop << endl;
    //   }
      
    //   if( currentState.stockMineral >= 100 + currentState.mineralsBooked - mineralsIn(goToBuild)
    // 	  &&
    // 	  currentState.mineralWorkers + currentState.gasWorkers > 0
    // 	  &&
    // 	  currentState.numberPylons > 0
    // 	  &&
    // 	  ( currentState.numberRefineries < currentState.numberBases
    // 	    || ( currentState.numberRefineries + 1 == currentState.numberBases
    // 		 && find_if( begin(currentState.busy),
    // 			     end(currentState.busy),
    // 			     [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} ) != currentState.busy.end() )
    // 	  )
    // 	)
    //   {
    // 	return true;
    //   }
    //   else
    // 	return false;
    // }
    // else
    // {
      // if(currentState.seconds > 700)
      // {
      // 	cout << "dep ok: " << dependenciesCheck( actionToDo.getFullName() ) << endl;
      // }


      if( ( actionToDo.getCostMineral() == 0 || currentState.stockMineral >= actionToDo.getCostMineral() + currentState.mineralsBooked - mineralsIn(goToBuild) )
	     &&
	     ( actionToDo.getCostGas() == 0 || currentState.stockGas >= actionToDo.getCostGas() + currentState.gasBooked - gasIn(goToBuild) ) 
	     &&
	     currentState.mineralWorkers + currentState.gasWorkers > 0
	     &&
	     currentState.numberPylons > 0
	     &&
	     dependenciesCheck( actionToDo.getFullName() )
	     )
    {
      return true;
    }
    else
      return false;
      // }
  }
  
  bool BuildOrderObjective::canHandleNotBuilding( const Action &actionToDo ) const
  {
    if( actionToDo.getType() == ActionType::building )
      return false;

    string creator = actionToDo.getCreator();
        
    if( ( actionToDo.getCostMineral() == 0 || currentState.stockMineral >= actionToDo.getCostMineral() + currentState.mineralsBooked )
	&&
	( actionToDo.getCostGas() == 0 || currentState.stockGas >= actionToDo.getCostGas() + currentState.gasBooked )
	&&
	currentState.supplyUsed + actionToDo.getCostSupply() <= currentState.supplyCapacity
	&&
	( creator.empty() || currentState.resources[ creator ].second > 0 )
	&&
	dependenciesCheck( actionToDo.getFullName() )
      )
    {
      return true;
    }
    else
      return false;
  }

  bool BuildOrderObjective::handleActionToDo( const Action &actionToDo ) const
  {
    // if the next action is building a building
    if( actionToDo.getType() == ActionType::building )
    {
      if( canHandleBuilding( actionToDo ) )
      {
	currentState.mineralsBooked += actionToDo.getCostMineral();
	currentState.gasBooked += actionToDo.getCostGas();
		
	currentState.inMove.push_back( ActionPrep( actionToDo.getData(), goToBuild, currentState.inMove.size() ) );
	if( currentState.mineralWorkers > 0 )
	  --currentState.mineralWorkers;
	else
	  --currentState.gasWorkers;

#ifndef NDEBUG
	string text = "Go for " + actionToDo.getFullName() + " at ";
	cout << std::left << setw(35) << text << setw(5) << currentState.seconds
	     << "  m = " << setw(9) << currentState.stockMineral
	     << "  g = " << setw(8) << currentState.stockGas
	     << "  mb = " << setw(5) << currentState.mineralsBooked
	     << "  gb = " << setw(4) << currentState.gasBooked
	     << "  mw = " << setw(3) << currentState.mineralWorkers
	     << "  gw = " << setw(3) << currentState.gasWorkers
	     << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif
	
	return true;
      }
    }
    // otherwise, it is a unit/research/upgrade
    else
    {
      string creator = actionToDo.getCreator();

      if( canHandleNotBuilding( actionToDo ) )
      {
	currentState.supplyUsed += actionToDo.getCostSupply();
	currentState.stockMineral -= actionToDo.getCostMineral();
	currentState.stockGas -= actionToDo.getCostGas();
	
	if( !creator.empty() && creator.compare("Protoss_Probe") != 0 )
	  --currentState.resources[ creator ].second;
	
	pushInBusy( actionToDo.getFullName() );
	
#ifndef NDEBUG
	string text = "Start " + actionToDo.getFullName() + " at ";
	cout << std::left << setw(35) << text << setw(5) << currentState.seconds
	     << "  m = " << setw(9) << currentState.stockMineral
	     << "  g = " << setw(8) << currentState.stockGas
	     << "  mb = " << setw(5) << currentState.mineralsBooked
	     << "  gb = " << setw(4) << currentState.gasBooked
	     << "  mw = " << setw(3) << currentState.mineralWorkers
	     << "  gw = " << setw(3) << currentState.gasWorkers
	     << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif
	
	return true;
      }
    }

    return false;
  }
  
  bool BuildOrderObjective::makingPylons() const
  {
    for( const auto &t : currentState.busy )
      if( t.name.compare("Protoss_Pylon") == 0 )
	return true;

    for( const auto &t : currentState.inMove )
      if( t.action.name.compare("Protoss_Pylon") == 0 )
	return true;    

    return false;
  }

  void BuildOrderObjective::youMustConstructAdditionalPylons() const
  {
    // build the first pylon ASAP
    if( currentState.numberPylons == 0 )
    {
      if( currentState.stockMineral >= 100 - mineralsIn( returnToMinerals ) )
      {
	currentState.inMove.push_back( ActionPrep( actionOf["Protoss_Pylon"], goToBuild, currentState.inMove.size() ) );
	
	currentState.mineralsBooked += 100;
	
	if( currentState.mineralWorkers > 0 )
	  --currentState.mineralWorkers;
	else
	  --currentState.gasWorkers;

#ifndef NDEBUG
	cout << std::left << setw(35) << "Go for first Protoss_Pylon at " << setw(5) << currentState.seconds
	     << "  m = " << setw(9) << currentState.stockMineral
	     << "  g = " << setw(8) << currentState.stockGas
	     << "  mb = " << setw(5) << currentState.mineralsBooked
	     << "  gb = " << setw(4) << currentState.gasBooked
	     << "  mw = " << setw(3) << currentState.mineralWorkers
	     << "  gw = " << setw(3) << currentState.gasWorkers
	     << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif	
      }
    }
    // otherwise build other pylons when needed
    else
    {
      int productionCapacity =
	currentState.resources["Protoss_Nexus"].first
	+ count_if( begin(currentState.busy), end(currentState.busy), [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;})

	+ 2 * ( currentState.resources["Protoss_Gateway"].first
		+ count_if( begin(currentState.busy), end(currentState.busy), [](ActionData &a){return a.name.compare( "Protoss_Gateway" ) == 0;}) )
	+ 4 * ( currentState.resources["Protoss_Robotics_Facility"].first
		+ count_if( begin(currentState.busy), end(currentState.busy), [](ActionData &a){return a.name.compare( "Protoss_Robotics_Facility" ) == 0;}) )
	+ 6 * ( currentState.resources["Protoss_Stargate"].first
		+ count_if( begin(currentState.busy), end(currentState.busy), [](ActionData &a){return a.name.compare( "Protoss_Stargate" ) == 0;}) );

      int plannedSupply = currentState.supplyCapacity
	+ 8 * count_if( begin(currentState.busy), end(currentState.busy), [](ActionData &a){return a.name.compare( "Protoss_Pylon" ) == 0;} )
	+ 8 * count_if( begin(currentState.inMove), end(currentState.inMove), [](ActionPrep &t){return t.action.name.compare("Protoss_Pylon") == 0;} )
	+ 9 * count_if( begin(currentState.busy), end(currentState.busy), [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} )
	+ 9 * count_if( begin(currentState.inMove), end(currentState.inMove), [](ActionPrep &t){return t.action.name.compare("Protoss_Nexus") == 0;} );

      if( plannedSupply <= productionCapacity + currentState.supplyUsed )
      {
	int countBuild = 0;
	do
	{
	  currentState.inMove.push_back( ActionPrep( actionOf["Protoss_Pylon"], goToBuild, currentState.inMove.size() ) );
	  currentState.mineralsBooked += 100;
	  ++countBuild;
	} while( plannedSupply + ( 8 * countBuild ) <= productionCapacity + currentState.supplyUsed );
		
	if( currentState.mineralWorkers > 0 )
	  --currentState.mineralWorkers;
	else
	  --currentState.gasWorkers;

#ifndef NDEBUG
	cout << std::left << setw(35) << "Go for Protoss_Pylon at " << setw(5) << currentState.seconds
	     << "  m = " << setw(9) << currentState.stockMineral
	     << "  g = " << setw(8) << currentState.stockGas
	     << "  mb = " << setw(5) << currentState.mineralsBooked
	     << "  gb = " << setw(4) << currentState.gasBooked
	     << "  mw = " << setw(3) << currentState.mineralWorkers
	     << "  gw = " << setw(3) << currentState.gasWorkers
	     << "  s = " << currentState.supplyUsed << "/" << currentState.supplyCapacity << ")" << endl;
#endif
      }
    }
  }

  void BuildOrderObjective::pushInBusy( const string  &name ) const
  {
    ActionData a  = actionOf[ name ];
    currentState.busy.push_back( a );
    
    // if( goal.compare("Protoss_Probe") != 0 && goal.compare("Protoss_Pylon") != 0 )
    // {
    bo.emplace_back( a.name, currentState.seconds, currentState.seconds + a.secondsRequired );
    if( goals.find( a.name ) != goals.end() )
      ++goals.at( a.name ).second;
    // }
  }

  bool BuildOrderObjective::dependenciesCheck( const string &s ) const
  {
    ActionData data = actionOf[ s ]; 
    if( data.costGas > 0 && currentState.numberRefineries == 0)
      return false;

    // Ok, this if statement is a bit tricky
    // return false if, for any dependency: 
    if( any_of( begin(data.dependencies), end(data.dependencies),
		[&](const string &n)
		{
		  // 1. the dependency has not been created yet
		  return ( currentState.resources[ n ].first == 0
			   &&
			   // 2. and (it is not planned to get one before goToBuild seconds OR data not a building)
			   ( data.actionType != ActionType::building
			     ||
			     none_of( begin(currentState.busy), end(currentState.busy),
				      [&n](ActionData &a)
		                      {
					return ( a.name.compare( n ) == 0 && a.secondsRequired <= goToBuild );
				      })
			   ) 
			 );
		} ) )
      return false;

    return true;
  }

  double BuildOrderObjective::sharpMineralsIn( int duration, int InSeconds ) const
  {
    double futurProduction = 0.;
    int workers = currentState.mineralWorkers;

    int min_time = std::min( InSeconds, 20 );
    vector<int> last_build;

    // simulation time from now till InSeconds
    for( int i = 1 ; i <= min_time ; ++i )
    {
      for( const auto &t : currentState.inMove )
	if( t.action.creator.compare("Protoss_Probe") == 0
	    &&
	    t.action.name.compare("Mineral") == 0
	    &&
	    t.waitTime - i == 0 )
	{
	  ++workers;
	}
      
      for( const auto &t : currentState.busy )
	if( t.name.compare("Protoss_Probe") == 0
	    &&
	    t.secondsRequired + 2 - i == 0 )
	{
	  ++workers;
	  last_build.push_back(i);
	}
    }

    for( int i = min_time + 1 ; i <= InSeconds ; ++i )
    {
      for( const auto &l : last_build )
	if( ( i + 2 - l ) % 20 == 0 )
	  ++workers;
    }

    // start to count income from InSeconds till InSeconds + duration
    for( int i = InSeconds + 1 ; i <= InSeconds + duration ; ++i )
    {
      for( const auto &l : last_build )
	if( ( i + 2 - l ) % 20 == 0 )
	  ++workers;
      
      futurProduction += workers * minRate;
    }
    
    return futurProduction;
  }
  
  double BuildOrderObjective::sharpGasIn( int duration, int InSeconds ) const
  {
    double futurProduction = 0.;
    int workers = currentState.gasWorkers;

    // simulation time from now till InSeconds
    for( int i = 1 ; i <= InSeconds ; ++i )
    {
      for( const auto &t : currentState.inMove )
	if( t.action.creator.compare("Protoss_Probe") == 0
	    &&
	    t.action.name.compare("Gas") == 0
	    &&
	    t.waitTime - i == 0 )
	{
	  ++workers;
	}
    }

    // start to count income from InSeconds till InSeconds + duration
    for( int i = InSeconds + 1 ; i <= InSeconds + duration ; ++i )
      futurProduction += workers * minRate;

    return futurProduction;
  }

  int BuildOrderObjective::v_heuristicVariable( const vector< int > &vecId, const vector< Action > *vecVariables, BuildOrderDomain *domain )
  {
    return vecId[ randomVar.getRandNum( vecId.size() ) ];
  }

  int BuildOrderObjective::v_heuristicValue( const std::vector< double > &vecGlobalCosts, 
					     double &bestEstimatedCost,
					     int &bestValue ) const
  {
    int best = 0;
    // double bestHelp = numeric_limits<int>::max();

    for( int i = 0; i < vecGlobalCosts.size(); ++i )
    {
      // if( vecGlobalCosts[i] < bestEstimatedCost
      // 	  || ( vecGlobalCosts[i] == bestEstimatedCost
      // 	       && vecGlobalCosts[i] < numeric_limits<int>::max()
      // 	       && heuristicValueHelper.at( i ) < bestHelp ) )
      if( vecGlobalCosts[i] < bestEstimatedCost )
      {
	bestEstimatedCost = vecGlobalCosts[i];
	bestValue = i;
	best = i;
      }
    }
    
    return best;
  }

  void BuildOrderObjective::v_setHelper( const Action &b, const vector< Action > *vecVariables, const BuildOrderDomain *domain )
  {
    // int pos = b.getValue();

    // // the idea here is to favor larger values: if you have to move a
    // // variable, move it as far as possible, in order to not disturb
    // // too much what has been done so far.
    // heuristicValueHelper.at( pos ) = domain->getSize() - pos;

    heuristicValueHelper.at( b.getValue() ) = randomVar.getRandNum( 1000 );
  }

  double BuildOrderObjective::v_postprocessSatisfaction( vector< Action > *vecVariables,
							 BuildOrderDomain *domain,
							 double &bestCost,
							 vector< Action > &bestSolution,
							 double sat_timeout ) const
  {
    chrono::time_point<chrono::high_resolution_clock> startPostprocess = chrono::high_resolution_clock::now(); 
    chrono::duration<double,micro> postprocesstimer(0);

    double cost = v_cost( vecVariables, domain );
    
    if( cost < bestCost )
    {
      bestBO = bo;
      bestCost = cost;
      copy( begin(*vecVariables), end(*vecVariables), begin(bestSolution) );
    }

    postprocesstimer = chrono::high_resolution_clock::now() - startPostprocess;
    return postprocesstimer.count();
  }


  double BuildOrderObjective::v_postprocessOptimization( vector< Action > *vecVariables,
							 BuildOrderDomain *domain,
							 double &bestCost,
							 double opt_timeout ) 
  {
    chrono::time_point<chrono::high_resolution_clock> startPostprocess = chrono::high_resolution_clock::now(); 
    chrono::duration<double,micro> postprocesstimer(0);

    vector<Action> copyVec(*vecVariables);
    double optiCost = costOpti( &copyVec );

    if( optiCost < bestCost )
    {
      bestBO = bo;
      bestCost = optiCost;
      vecVariables->clear();
      for( const auto &v : copyVec )
	vecVariables->push_back(v);
      //copy( begin(copyVec), end(copyVec), begin(*vecVariables) );
    }

    printBO();

    postprocesstimer = chrono::high_resolution_clock::now() - startPostprocess;
    return postprocesstimer.count();
  }

  void BuildOrderObjective::makeVecVariables( const pair<string, int> &input, vector<Action> &variables, map< string, pair<int, int> > &goals )
  {
    Action action( actionOf[input.first] );
    goals.emplace( action.getFullName(), make_pair<int, int>( static_cast<int>( input.second ), 0 ) );
    
    makeVecVariables( action, variables, input.second );
  }

  void BuildOrderObjective::makeVecVariables( const Action &action, vector<Action> &variables, int count )
  {
    string name = action.getFullName();
    
    if( count > 0 )
    {
      variables.push_back( action );
      for( int i = 1; i < count; ++i )
    	variables.push_back( Action( actionOf[ action.getFullName() ] ) );
      
      for( const auto &d : action.getDependencies() )
    	if( d.compare( "Protoss_High_Templar" ) == 0 || d.compare( "Protoss_Dark_Templar" ) == 0 )
    	  makeVecVariables( Action( actionOf[ d ] ), variables, 2 * count ); // Each (dark) archon needs 2 (dark) templars 
    	else
    	  if( d.compare( "Protoss_Nexus" ) != 0 )
	  {
	    if( none_of( begin(variables), end(variables), [&d](Action &a){return d.compare(a.getFullName()) == 0;} ) )
	      makeVecVariables( Action( actionOf[ d ] ), variables, 1 );
	  }

      if( action.getCostGas() > 0 )
      {
	if( none_of( begin(variables), end(variables), [](Action &a){return a.getFullName().compare("Protoss_Assimilator") == 0;} ) )
	{
	  makeVecVariables( Action( actionOf[ "Protoss_Assimilator" ] ), variables, 1 );
	}
      }
    }
  }
  
  /*******************/
  /* MakeSpanMinCost */
  /*******************/
  MakeSpanMinCost::MakeSpanMinCost() : BuildOrderObjective( "MakeSpanMinCost" ) { }
  MakeSpanMinCost::MakeSpanMinCost( const vector< pair<string, int> > &input, vector<Action> &variables )
    : BuildOrderObjective( "MakeSpanMinCost", input, variables ) { }

  
  // double MakeSpanMinCost::v_postprocessOptimization( vector< Action > *vecVariables, BuildOrderDomain *domain, double &bestCost )
  // {

  //   return 0;
  // }

  
  /*******************/
  /* MakeSpanMaxProd */
  /*******************/
  MakeSpanMaxProd::MakeSpanMaxProd() : BuildOrderObjective( "MakeSpanMaxProd" ) { }
  MakeSpanMaxProd::MakeSpanMaxProd( const vector< pair<string, int> > &input, vector<Action> &variables )
    : BuildOrderObjective( "MakeSpanMaxProd", input, variables ) { }

  // double MakeSpanMaxProd::v_postprocessOptimization( vector< Action > *vecVariables, BuildOrderDomain *domain, double &bestCost )
  // {

  //   return 0;
  // }
}
