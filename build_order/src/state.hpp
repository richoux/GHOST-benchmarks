#pragma once

#include "actionData.hpp"
#include "actionPrep.hpp"
#include "actionMap.hpp"

struct State
{
  int		seconds;
  double	stock_mineral;
  double	stock_gas;
  int		minerals_booked;
  int		gas_booked;
  int		mineral_workers;
  int		gas_workers;
  int		supply_used;
  int		supply_capacity;
  int		number_bases;
  int		number_refineries;
  int		number_pylons;

  // <name, [total number, number available now]>
  // <Gateway, [2,1]> means we have 2 finished Gateways but only one available to produce something right now.
  map<string, pair<int, int> > resources;

  // vector of actions in progress
  // for instance, a pylon is warping, so it is not available yet
  vector< ActionData > busy;

  // vector of actions in preparation
  // for instance, a worker is moving to warp a pylon somewhere, so warping didn't start yet.
  vector< ActionPrep > in_move;      

  State()
    : seconds(0),
      stock_mineral(0.),
      stock_gas(0.),
      minerals_booked(0),
      gas_booked(0),
      mineral_workers(0),
      gas_workers(0),
      supply_used(5),
      supply_capacity(9),
      number_bases(1),
      number_refineries(0),
      number_pylons(0),
      resources(),
      busy{action_of["Protoss_Probe"]}, // make a worker directly
      in_move{ ActionPrep{ action_of["Protoss_Mineral"], from_base_to_minerals, 0 }, // all initial workers go mining
	  ActionPrep{ action_of["Protoss_Mineral"], from_base_to_minerals, 1 },
	    ActionPrep{ action_of["Protoss_Mineral"], from_base_to_minerals, 2 },
	      ActionPrep{ action_of["Protoss_Mineral"], from_base_to_minerals, 3 } }
  { }
      
  State( int seconds,
	 double stock_mineral,
	 double stock_gas,
	 int minerals_booked,
	 int gas_booked,
	 int mineral_workers,
	 int gas_workers,
	 int supply_used,
	 int supply_capacity,
	 int number_bases,
	 int number_refineries,
	 int number_pylons,
	 const map<string, pair<int, int> > &resources,
	 const vector< ActionData > &busy,
	 const vector< ActionPrep > &in_move )
    : seconds(seconds),
      stock_mineral(stock_mineral),
      stock_gas(stock_gas),
      minerals_booked(minerals_booked),
      gas_booked(gas_booked),
      mineral_workers(mineral_workers),
      gas_workers(gas_workers),
      supply_used(supply_used),
      supply_capacity(supply_capacity),
      number_bases(number_bases),
      number_refineries(number_refineries),
      number_pylons(number_pylons),
      resources(resources),
      busy(busy),
      in_move(in_move)
  { }

  void reset()
  {
    seconds		= 0;
    stock_mineral	= 0.;
    stock_gas		= 0.;
    minerals_booked	= 0;
    gas_booked		= 0;
    mineral_workers	= 0;
    gas_workers		= 0;
    supply_used		= 5;
    supply_capacity	= 9;
    number_bases	= 1;
    number_refineries	= 0;
    number_pylons	= 0;
    resources.clear();
    resources["Protoss_Nexus"].first = 1;
    resources["Protoss_Nexus"].second = 0;	
    busy.clear();
    busy.push_back( action_of["Protoss_Probe"] ); // make a worker directly
    in_move.clear();
    for( int i = 0 ; i < 4 ; ++i ) // all initial workers go mining
      in_move.push_back( ActionPrep{ action_of["Protoss_Mineral"], from_base_to_minerals, i } );
  }
};
