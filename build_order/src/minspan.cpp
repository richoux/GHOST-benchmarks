#include <algorithm>
#include <limits>
#include <cmath>
#include <chrono>
#include <ctime>
#include <numeric>
#include <iomanip>

#include "minspan.hpp"

MinSpan::MinSpan( const vector< ActionData >& actions )
  : Objective( "MinSpan" ),
    current_state( State() ),
    goals( map<string, pair<int, int> >() ),
    bo( vector<BO>() ),
    actions(actions)
{
  // for( const auto &i : input)
  //   makeVecVariables( i, variables, goals );
}

void MinSpan::printBO() const
{
  cout << "\n\n";
  for( const auto &b : bestBO )
  {
    if( b.full_name.compare("Protoss_Probe") != 0 && b.full_name.compare("Protoss_Pylon") != 0 )


      cout << b.full_name
	   << ": start at " << b.start_time
	   << ", finish at " << b.completed_time << "\n";
  }
  cout << "\n";
}
  
double MinSpan::required_cost( const vector< Variable >& vecVariables ) const
{
  current_state.reset();
  bo.clear();

  // we consider we start with 0 units/buildings we are looking to produce
  for( auto &g : goals)
    g.second.second = 0;

  int i = 0;
  while( i < actions.size() || !current_state.busy.empty() )
  {
    auto& action_to_do = actions[i];
    // cout << "vec size: " << vecVariables->size()
    // 	   << ", iter: " << std::distance( actionToDo, vecVariables->end() )
    // 	   << ", busy: " << current_state.busy.size() << endl;

    // if( current_state.seconds > 700 )
    // {
    // 	for( auto itit = actionToDo ; itit != vecVariables->end() ; ++itit )
    // 	  cout << itit->getFullName() << "(" << can_handle_not_building( *itit ) << ") ";
    // 	cout << endl;	
    // }
      
    ++current_state.seconds;

    // update mineral / gas stocks
    current_state.stock_mineral += current_state.mineral_workers * mineral_rate; // minRate mineral per worker per second in average
    current_state.stock_gas += current_state.gas_workers * gas_rate; // gasRate gas per worker per second in average

    // update busy list
    update_busy();

    // update in_move list
    update_in_move();

    // if( action_to_do != vecVariables->end() )
    if( i < actions.size() )
    {
      deal_with_workers();
	
      // build a pylon if I must, ie:
      // 1. if I am not currently making pylons
      // 2. if my supply cap cannot manage the next global unit production
      if( !making_pylons() || current_state.number_pylons != 0 )
	you_must_construct_additional_pylons();

      // can I produce units?
      if( action_to_do.actionType != ActionType::unit )
	produce_units_first( action_to_do, vecVariables );
	
      // can I handle the current action?
      if( handle_action_to_do( action_to_do ) )
	++i;
      else // can I handle the next action?
      {
	// if( nextAction != vecVariables->end() )
	if( i < actions.size() -1 )
	{
	  auto& next_action = actions[i + 1];

	  // book resources for the current action
	  int mineral_cost = action_to_do.cost_mineral;
	  int gas_cost = action_to_do.cost_gas;
	    
	  current_state.minerals_booked += mineral_cost;
	  current_state.gas_booked += gas_cost;
	  if( can_handle_building( next_action ) || can_handle_not_building( next_action ) )
	  {
	    // cout << "Swap " << actionToDo->getFullName() << ":" << actionToDo->getValue()
	    // 	   << " with " << nextAction->getFullName() << ":" << nextAction->getValue() << endl;

	    // std::swap( action_to_do, next_action );
	    action_to_do.swap( next_action );
	    current_state.minerals_booked -= mineral_cost;
	    current_state.gas_booked -= gas_cost;
	    if( handle_action_to_do( action_to_do ) )
	      ++i;
	    else
	    {
	      cout << "This should never append.\n";
	      exit( 0 );
	    }	      
	  }
	  else
	  {
	    current_state.minerals_booked -= mineral_cost;
	    current_state.gas_booked -= gas_cost;
	  }
	}
      }
    }
  }
  return static_cast<double>( current_state.seconds );
}

double MinSpan::cost_optimization( const vector< Variable >& vecVariables ) const
{
  current_state.reset();
  bo.clear();
  for( auto &g : goals)
    g.second.second = 0;

  vector< Variable > copyVec = vecVariables;
    
  // auto actionToDo = copyVec.begin();
  int i = 0;
  // while( actionToDo != copyVec.end() || !current_state.busy.empty() )
  while( i < copyVec.size() || !current_state.busy.empty() )
  {
    ++current_state.seconds;

    // update mineral / gas stocks
    current_state.stock_mineral += current_state.mineral_workers * mineral_rate;
    current_state.stock_gas += current_state.gas_workers * gas_rate;

    // update busy list
    update_busy();

    // update inMove list
    update_in_move();

    // if( action_to_do != copyVec.end() )
    if( i < copyVec.size() )
    {
      deal_with_workers();
	
      // only used by postprocessingOptimization, to see if we can
      // shorten the makespan by making more production buildings,
      // like gateways for instance.
      ActionData action, creator;
      double real_time, simulated_time;
      int simulated_mineral, simulated_gas;
      int future_mineral, future_gas;
	  
      int to_produce;
      int creator_in_production;
      int total_number;
      for( const auto &g : goals )
      {
	action = action_of[ g.first ];
	if( action.actionType == ActionType::building )
	  continue;

	if( action.name.compare("Protoss_Archon") == 0 || action.name.compare("Protoss_Dark_Archon") == 0 )
	  continue;
	    
	creator = action_of[ action.creator ];

	creator_in_production =
	  count_if( begin(current_state.in_move),
		    end(current_state.in_move),
		    [&creator](ActionPrep& t){return t.action.name.compare( creator.name ) == 0;})
	  + count_if( begin(current_state.busy),
		      end(current_state.busy),
		      [&creator](ActionData& a){return a.name.compare( creator.name ) == 0;});

	total_number = current_state.resources[creator.name].first + creator_in_production;
	    
	if( total_number == 0 )
	  continue;
	
	// test is we are faster after making an additional production building
	to_produce = g.second.first - g.second.second;
	real_time = 0.;
	    
	for( const auto &t : current_state.busy )
	  if( t.name.compare( action.name ) == 0 )
	  {
	    --to_produce;
	    real_time += t.seconds_required;
	  }

	if( to_produce <= 0 )
	  continue;

	simulated_time = real_time + creator.seconds_required;
	real_time += to_produce * action.seconds_required / ( current_state.resources[creator.name].first + creator_in_production );
	simulated_time += to_produce * action.seconds_required / ( current_state.resources[creator.name].first + creator_in_production + 1 );

	// real_time: estimation of time needed to produce goal g with current production building
	// simulated_time: same estimation if we have one additional production building
	
	if( simulated_time > real_time )
	  continue;

	// test is we have enough money for making an additional production building
	simulated_mineral = ( current_state.resources[creator.name].first + creator_in_production + 1 ) * action.costMineral;
	simulated_gas = ( current_state.resources[creator.name].first + creator_in_production + 1 ) * action.costGas;

	future_mineral = sharp_minerals_in( action.seconds_required, creator.seconds_required );
	future_gas = sharp_gas_in( action.seconds_required, creator.seconds_required );

	// if we can make this additional building, do it! 
	if( future_mineral >= simulated_mineral && future_gas >= simulated_gas
	    &&
	    ( creator.cost_mineral == 0 || current_state.stock_mineral >= creator.cost_mineral + current_state.minerals_booked - minerals_in( go_to_build ) )
	    &&
	    ( creator.cost_gas == 0 || current_state.stock_gas >= creator.cost_gas + current_state.gas_booked - gas_in( go_to_build ) ) 
	    &&
	    current_state.mineral_workers + current_state.gas_workers > 0
	    &&
	    current_state.number_pylons > 0
	    )
	{
	  current_state.minerals_booked += creator.cost_mineral;
	  current_state.gas_booked += creator.cost_gas;
	    
	  actions.emplace_back( ActionData( creator ) );
	  // Create a new variables vector, with a new variable at the end and all domains incremented
	  int new_size = (int)vecVariables.size() + 1;
	  // Warning: loop until new_size - 1 for the current variables vector
	  for( int i = 0; i < new_size - 1; ++i )
	  {
	    auto var = vecVariables[ i ];
	    vecVariables[ i ] = Variable( var.get_name(), var.get_short_name(), -1, new_size );
	    vecVariables[ i ].set_value( var.get_value() + 1 );
	  }
	  vecVariables.emplace_back( creator.name, creator.name, -1, new_size );
	  vecVariables[ new_size - 1 ].set_value( i );
	  
	  current_state.in_move.push_back( ActionPrep( creator, go_to_build, current_state.in_move.size() ) );
	  if( current_state.mineral_workers > 0 )
	    --current_state.mineral_workers;
	  else
	    --current_state.gas_workers;

#ifndef NDEBUG
	  string text = "Optimize " + creator.name + " at ";
	  cout << std::left << setw(35) << text << setw(5) << current_state.seconds
	       << "  m = " << setw(9) << current_state.stock_mineral
	       << "  g = " << setw(8) << current_state.stock_gas
	       << "  mb = " << setw(5) << current_state.minerals_booked
	       << "  gb = " << setw(4) << current_state.gas_booked
	       << "  mw = " << setw(3) << current_state.mineral_workers
	       << "  gw = " << setw(3) << current_state.gas_workers
	       << "  s = " << current_state.supply_used << "/" << current_state.supply_capacity << ")\n";
#endif	    
	}	    
      }	  
	
      // build a pylon if I must, ie:
      // 1. if I am not currently making pylons
      // 2. if my supply cap cannot manage the next global unit production
      if( !making_pylons() || current_state.number_pylons != 0 )
	you_must_construct_additional_pylons();

      // can I produce units?
      if( actions[ i ].actionType != ActionType::unit )
	produce_units_first( actions[ i ], &copyVec );

      // can I handle the current action?
      if( handle_action_to_do( actions[ i ] ) )
	++i;
      else // can I handle the next action?
      {
	// auto next_action = action_to_do + 1;
	if( i + 1 < copyVec.size() )
	{
	  // book resources for the current action
	  int mineral_cost = actions[ i ].cost_mineral;
	  int gas_cost = actions[ i ].cost_gas;
	    
	  current_state.minerals_booked += mineral_cost;
	  current_state.gas_booked += gas_cost;
	  if( can_handle_building( actions[ i + 1 ] ) || can_handle_not_building( actions[ i + 1 ] ) )
	  {
	    // cout << "Swap " << actionToDo->getFullName() << ":" << actionToDo->getValue()
	    // 	   << " with " << nextAction->getFullName() << ":" << nextAction->getValue() << endl;

	    std::swap( actions[ i ], actions[ i + 1 ] );
	    current_state.minerals_booked -= mineral_cost;
	    current_state.gas_booked -= gas_cost;
	    if( handle_action_to_do( actions[ i ] ) )
	      ++i;
	    else
	    {
	      cout << "This should never append." << endl;
	      exit(0);
	    }	      
	  }
	  else
	  {
	    current_state.minerals_booked -= mineral_cost;
	    current_state.gas_booked -= gas_cost;
	  }
	}
      }
    }
  }
  return static_cast<double>( current_state.seconds );
}

void MinSpan::update_busy() const
{
  for( auto &t : current_state.busy )
  {
    int time = t.decrease_seconds();
    if( time == 0 )
    {
      // if creator is something else than a probe, signal it as available
      // (recall: a probe is directly available after starting a building warping).
      if( t.creator.compare("Protoss_Probe") != 0 )
      {
	++current_state.resources[ t.creator ].second;
      }

      // if the action was producing a probe, send it gathering minerals
      if( t.name.compare("Protoss_Probe") == 0 )
	current_state.in_move.push_back( ActionPrep( action_of["Protoss_Mineral"], from_base_to_minerals, current_state.in_move.size() ) );
      else
      {
	if( t.name.compare("Protoss_Nexus") == 0 )
	{
	  ++current_state.resources["Protoss_Nexus"].first;
	  ++current_state.resources["Protoss_Nexus"].second;
	  current_state.supply_capacity += 9;
	  ++current_state.number_bases;
	}
	else if( t.name.compare("Protoss_Pylon") == 0 )
	{
	  current_state.supply_capacity += 8;
	  ++current_state.number_pylons;
	}
	else if( t.name.compare("Protoss_Assimilator") == 0 )
	{
	  ++current_state.number_refineries;
	    
	  // if we have few workers mining, do not sent them to gas
	  for( int i = 0; i < min( 3, current_state.mineral_workers - 3 ); ++i )
	  {
	    current_state.in_move.push_back( ActionPrep( action_of["Protoss_Gas"], from_min_to_gas, current_state.in_move.size() ) );
	    --current_state.mineral_workers;
	  }
	}
	else if( t.actionType == ActionType::building )
	{
	  ++current_state.resources[ t.name ].first;
	  ++current_state.resources[ t.name ].second;
	}
	else if( t.name.compare("Protoss_High_Templar") == 0
		 || t.name.compare("Protoss_Dark_Templar") == 0 )
	{
	  ++current_state.resources[ t.name ].first;
	  ++current_state.resources[ t.name ].second;
	}
      }

#ifndef NDEBUG
      string text = "Finish " + t.name + " at ";
      cout << std::left << setw(35) << text << setw(5) << current_state.seconds
	   << "  m = " << setw(9) << current_state.stock_mineral
	   << "  g = " << setw(8) << current_state.stock_gas
	   << "  mb = " << setw(5) << current_state.minerals_booked
	   << "  gb = " << setw(4) << current_state.gas_booked
	   << "  mw = " << setw(3) << current_state.mineral_workers
	   << "  gw = " << setw(3) << current_state.gas_workers
	   << "  s = " << current_state.supply_used << "/" << current_state.supply_capacity << ")\n";
#endif
    }
  }

  auto itEnd = remove_if( begin( current_state.busy ), end( current_state.busy ), [](ActionData &a){return a.seconds_required == 0;} );
  current_state.busy.erase( itEnd, end( current_state.busy ) );
}
  
void MinSpan::update_in_move() const
{
  // (Me in 2014) The code below is not groovy, but it is written that way to avoid a really weird bug
  // (Me in 2018, reading the comment above) Dude, seriously...
  for_each( begin( current_state.in_move ), end( current_state.in_move ), [](ActionPrep &a){ if(a.wait_time > 0) --a.wait_time; } );
  auto copy_in_move(current_state.in_move);
    
  for( int i = 0; i < copy_in_move.size(); ++i )
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
      
    if( copy_in_move[i].wait_time == 0
	&& ( copy_in_move[i].action.cost_mineral == 0 || current_state.stock_mineral >= copy_in_move[i].action.cost_mineral )
	&& ( copy_in_move[i].action.cost_gas == 0 || current_state.stock_gas >= copy_in_move[i].action.cost_gas )
	)
    {
      string creator = copy_in_move[i].action.creator;
      string goal = copy_in_move[i].action.name;

      int mineral_cost = copy_in_move[i].action.cost_mineral;
      int gas_cost = copy_in_move[i].action.cost_gas;

      if( creator.compare("Protoss_Probe") == 0 )
      {
	// The action is about to be done, so erase it from in_move
	auto to_erase = std::find_if( begin( current_state.in_move ), end( current_state.in_move ),
				     [&](ActionPrep &a){ return copy_in_move[i].id == a.id; });
	for_each( to_erase, current_state.in_move.end(), [](ActionPrep &a){ --a.id; } );
	for_each( begin( copy_in_move ) + i, end( copy_in_move ), [](ActionPrep &a){ --a.id; } );
	current_state.in_move.erase( to_erase );
	  
	if( goal.compare("Mineral") == 0 ) 
	  ++current_state.mineral_workers;
	else if( goal.compare("Gas") == 0 ) 
	  ++current_state.gas_workers;
	else // ie, the worker is about to build something
	{
	  push_in_busy( goal );
	  // warp building and return to mineral fields
	  current_state.in_move.push_back( ActionPrep( action_of["Protoss_Mineral"], return_to_minerals, current_state.in_move.size() ) );
	    
	  current_state.stock_mineral -= mineral_cost;
	  current_state.stock_gas -= gas_cost;
	    
	  current_state.minerals_booked -= mineral_cost;
	  current_state.gas_booked -= gas_cost;
	    
#ifndef NDEBUG
	  string text = "Start " + goal + " at ";
	  cout << std::left << setw(35) << text << setw(5) << current_state.seconds
	       << "  m = " << setw(9) << current_state.stock_mineral
	       << "  g = " << setw(8) << current_state.stock_gas
	       << "  mb = " << setw(5) << current_state.minerals_booked
	       << "  gb = " << setw(4) << current_state.gas_booked
	       << "  mw = " << setw(3) << current_state.mineral_workers
	       << "  gw = " << setw(3) << current_state.gas_workers
	       << "  s = " << current_state.supply_used << "/" << current_state.supply_capacity << ")\n";
#endif
	} 
      }
    }
  }    
}

void MinSpan::deal_with_workers() const
{
  // send workers to gas, if need and possible
  if( current_state.gas_workers + count_if( begin(current_state.in_move),
					  end(current_state.in_move),
					  [](ActionPrep &t){return t.action.name.compare("Gas") == 0;} )
      < current_state.number_refineries * 3 )
  {
    // if we have few workers mining, do not sent them to gas
    for( int i = 0 ; i < min( 3, current_state.mineral_workers - 3 ) ; ++i )
    {
      current_state.in_move.push_back( ActionPrep( actionOf["Protoss_Gas"], from_min_to_gas, current_state.in_move.size() ) );
      --current_state.mineral_workers;
    }
  }
    
  // produce a worker if I can, ie:
  // 1. if I have at least 50 minerals
  // 2. if I have at least one available Nexus
  // 3. if I am not supply blocked
  // 4. if I don't reach the saturation number (ie 24 workers per base)
  if( current_state.stock_mineral >= 50
      &&
      current_state.resources["Protoss_Nexus"].second > 0
      &&
      current_state.supply_used < current_state.supply_capacity
      &&
      current_state.mineral_workers + count_if( begin(current_state.in_move),
					      end(current_state.in_move),
					      [](const ActionPrep &t){return t.action.creator.compare("Protoss_Probe") == 0;})
      < current_state.number_bases * 24 )
  {
    current_state.stock_mineral -= 50;
    ++current_state.supply_used;
    --current_state.resources["Protoss_Nexus"].second;
    push_in_busy( "Protoss_Probe" );
      
#ifndef NDEBUG
    cout << std::left << setw(35) << "Start Protoss_Probe at " << setw(5) << current_state.seconds
	 << "  m = " << setw(9) << current_state.stock_mineral
	 << "  g = " << setw(8) << current_state.stock_gas
	 << "  mb = " << setw(5) << current_state.minerals_booked
	 << "  gb = " << setw(4) << current_state.gas_booked
	 << "  mw = " << setw(3) << current_state.mineral_workers
	 << "  gw = " << setw(3) << current_state.gas_workers
	 << "  s = " << current_state.supply_used << "/" << current_state.supply_capacity << ")\n";
#endif
  }
}

void MinSpan::produce_units_first( vector<Action>::iterator &actionToDo, vector< Action > *vecVariables ) const
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
	cout << "This should never append too.\n";
	exit(0);
      }	
    }
  }
}
  
bool MinSpan::can_handle_building( const Action &actionToDo ) const
{
  if( actionToDo.getType() != ActionType::building )
    return false;

  // special case for the Assimilator
  // if( actionToDo.getFullName().compare("Protoss_Assimilator") == 0 )
  // {
      
  //   if(current_state.seconds > 700)
  //   {
  //   	bool plop = find_if( begin(current_state.busy),
  //   			     end(current_state.busy),
  //   			     [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} ) != current_state.busy.end();
	
  //   	cout << "nb ref: " << current_state.numberRefineries
  //   	     << ", nb base:" << current_state.numberBases
  //   	     << ", under build: " << plop << endl;
  //   }
      
  //   if( current_state.stockMineral >= 100 + current_state.mineralsBooked - mineralsIn(goToBuild)
  // 	  &&
  // 	  current_state.mineralWorkers + current_state.gasWorkers > 0
  // 	  &&
  // 	  current_state.numberPylons > 0
  // 	  &&
  // 	  ( current_state.numberRefineries < current_state.numberBases
  // 	    || ( current_state.numberRefineries + 1 == current_state.numberBases
  // 		 && find_if( begin(current_state.busy),
  // 			     end(current_state.busy),
  // 			     [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} ) != current_state.busy.end() )
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
  // if(current_state.seconds > 700)
  // {
  // 	cout << "dep ok: " << dependenciesCheck( actionToDo.getFullName() ) << endl;
  // }


  if( ( actionToDo.getCostMineral() == 0 || current_state.stockMineral >= actionToDo.getCostMineral() + current_state.mineralsBooked - mineralsIn(goToBuild) )
      &&
      ( actionToDo.getCostGas() == 0 || current_state.stockGas >= actionToDo.getCostGas() + current_state.gasBooked - gasIn(goToBuild) ) 
      &&
      current_state.mineralWorkers + current_state.gasWorkers > 0
      &&
      current_state.numberPylons > 0
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
  
bool MinSpan::can_handle_not_building( const Action &actionToDo ) const
{
  if( actionToDo.getType() == ActionType::building )
    return false;

  string creator = actionToDo.getCreator();
        
  if( ( actionToDo.getCostMineral() == 0 || current_state.stockMineral >= actionToDo.getCostMineral() + current_state.mineralsBooked )
      &&
      ( actionToDo.getCostGas() == 0 || current_state.stockGas >= actionToDo.getCostGas() + current_state.gasBooked )
      &&
      current_state.supplyUsed + actionToDo.getCostSupply() <= current_state.supplyCapacity
      &&
      ( creator.empty() || current_state.resources[ creator ].second > 0 )
      &&
      dependenciesCheck( actionToDo.getFullName() )
      )
  {
    return true;
  }
  else
    return false;
}

bool MinSpan::handle_action_to_do( const Action &actionToDo ) const
{
  // if the next action is building a building
  if( actionToDo.getType() == ActionType::building )
  {
    if( canHandleBuilding( actionToDo ) )
    {
      current_state.mineralsBooked += actionToDo.getCostMineral();
      current_state.gasBooked += actionToDo.getCostGas();
		
      current_state.inMove.push_back( ActionPrep( actionToDo.getData(), goToBuild, current_state.inMove.size() ) );
      if( current_state.mineralWorkers > 0 )
	--current_state.mineralWorkers;
      else
	--current_state.gasWorkers;

#ifndef NDEBUG
      string text = "Go for " + actionToDo.getFullName() + " at ";
      cout << std::left << setw(35) << text << setw(5) << current_state.seconds
	   << "  m = " << setw(9) << current_state.stockMineral
	   << "  g = " << setw(8) << current_state.stockGas
	   << "  mb = " << setw(5) << current_state.mineralsBooked
	   << "  gb = " << setw(4) << current_state.gasBooked
	   << "  mw = " << setw(3) << current_state.mineralWorkers
	   << "  gw = " << setw(3) << current_state.gasWorkers
	   << "  s = " << current_state.supplyUsed << "/" << current_state.supplyCapacity << ")\n";
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
      current_state.supplyUsed += actionToDo.getCostSupply();
      current_state.stockMineral -= actionToDo.getCostMineral();
      current_state.stockGas -= actionToDo.getCostGas();
	
      if( !creator.empty() && creator.compare("Protoss_Probe") != 0 )
	--current_state.resources[ creator ].second;
	
      pushInBusy( actionToDo.getFullName() );
	
#ifndef NDEBUG
      string text = "Start " + actionToDo.getFullName() + " at ";
      cout << std::left << setw(35) << text << setw(5) << current_state.seconds
	   << "  m = " << setw(9) << current_state.stockMineral
	   << "  g = " << setw(8) << current_state.stockGas
	   << "  mb = " << setw(5) << current_state.mineralsBooked
	   << "  gb = " << setw(4) << current_state.gasBooked
	   << "  mw = " << setw(3) << current_state.mineralWorkers
	   << "  gw = " << setw(3) << current_state.gasWorkers
	   << "  s = " << current_state.supplyUsed << "/" << current_state.supplyCapacity << ")\n";
#endif
	
      return true;
    }
  }

  return false;
}
  
bool MinSpan::making_pylons() const
{
  for( const auto &t : current_state.busy )
    if( t.name.compare("Protoss_Pylon") == 0 )
      return true;

  for( const auto &t : current_state.in_move )
    if( t.action.name.compare("Protoss_Pylon") == 0 )
      return true;    

  return false;
}

void MinSpan::you_must_construct_additional_pylons() const
{
  // build the first pylon ASAP
  if( current_state.number_pylons == 0 )
  {
    if( current_state.stock_mineral >= 100 - minerals_in( return_to_minerals ) )
    {
      current_state.in_move.push_back( ActionPrep( action_of["Protoss_Pylon"], go_to_build, current_state.in_move.size() ) );
	
      current_state.minerals_booked += 100;
	
      if( current_state.mineral_workers > 0 )
	--current_state.mineral_workers;
      else
	--current_state.gas_workers;

#ifndef NDEBUG
      cout << std::left << setw(35) << "Go for first Protoss_Pylon at " << setw(5) << current_state.seconds
	   << "  m = " << setw(9) << current_state.stock_mineral
	   << "  g = " << setw(8) << current_state.stock_gas
	   << "  mb = " << setw(5) << current_state.minerals_booked
	   << "  gb = " << setw(4) << current_state.gas_booked
	   << "  mw = " << setw(3) << current_state.mineral_workers
	   << "  gw = " << setw(3) << current_state.gas_workers
	   << "  s = " << current_state.supply_used << "/" << current_state.supply_capacity << ")\n";
#endif	
    }
  }
  // otherwise build other pylons when needed
  else
  {
    int production_capacity =
      current_state.resources["Protoss_Nexus"].first
      + count_if( begin(current_state.busy), end(current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;})

      + 2 * ( current_state.resources["Protoss_Gateway"].first
	      + count_if( begin(current_state.busy), end(current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Gateway" ) == 0;}) )
      + 4 * ( current_state.resources["Protoss_Robotics_Facility"].first
	      + count_if( begin(current_state.busy), end(current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Robotics_Facility" ) == 0;}) )
      + 6 * ( current_state.resources["Protoss_Stargate"].first
	      + count_if( begin(current_state.busy), end(current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Stargate" ) == 0;}) );

    int planned_supply = current_state.supply_capacity
      + 8 * count_if( begin(current_state.busy), end(current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Pylon" ) == 0;} )
      + 8 * count_if( begin(current_state.in_move), end(current_state.in_move), [](ActionPrep &t){return t.action.name.compare("Protoss_Pylon") == 0;} )
      + 9 * count_if( begin(current_state.busy), end(current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} )
      + 9 * count_if( begin(current_state.in_move), end(current_state.in_move), [](ActionPrep &t){return t.action.name.compare("Protoss_Nexus") == 0;} );

    ///////////////////////////////
    ///////////////////////////////
    ///////////////////////////////
    ///////////////////////////////
    ///////////////////////////////

    
    if( planned_supply <= production_capacity + current_state.supply_used )
    {
      int count_build = 0;
      do
      {
	current_state.in_move.push_back( ActionPrep( action_of["Protoss_Pylon"], go_to_build, current_state.in_move.size() ) );
	current_state.minerals_booked += 100;
	++count_build;
      } while( planned_supply + ( 8 * count_build ) <= production_capacity + current_state.supply_used );
		
      if( current_state.mineral_workers > 0 )
	--current_state.mineral_workers;
      else
	--current_state.gas_workers;

#ifndef NDEBUG
      cout << std::left << setw(35) << "Go for Protoss_Pylon at " << setw(5) << current_state.seconds
	   << "  m = " << setw(9) << current_state.stock_mineral
	   << "  g = " << setw(8) << current_state.stock_gas
	   << "  mb = " << setw(5) << current_state.minerals_booked
	   << "  gb = " << setw(4) << current_state.gas_booked
	   << "  mw = " << setw(3) << current_state.mineral_workers
	   << "  gw = " << setw(3) << current_state.gas_workers
	   << "  s = " << current_state.supply_used << "/" << current_state.supply_capacity << ")\n";
#endif
    }
  }
}

void MinSpan::push_in_busy( const string& name ) const
{
  ActionData a  = action_of[ name ];
  current_state.busy.push_back( a );
    
  bo.emplace_back( a.name, current_state.seconds, current_state.seconds + a.secondsRequired );
  if( goals.find( a.name ) != goals.end() )
    ++goals.at( a.name ).second;
}

bool MinSpan::dependenciesCheck( const string &s ) const
{
  ActionData data = actionOf[ s ]; 
  if( data.costGas > 0 && current_state.numberRefineries == 0)
    return false;

  // Ok, this if statement is a bit tricky
  // return false if, for any dependency: 
  if( any_of( begin(data.dependencies), end(data.dependencies),
	      [&](const string &n)
	      {
		// 1. the dependency has not been created yet
		return ( current_state.resources[ n ].first == 0
			 &&
			 // 2. and (it is not planned to get one before goToBuild seconds OR data not a building)
			 ( data.actionType != ActionType::building
			   ||
			   none_of( begin(current_state.busy), end(current_state.busy),
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

double MinSpan::sharpMineralsIn( int duration, int InSeconds ) const
{
  double futurProduction = 0.;
  int workers = current_state.mineralWorkers;

  int min_time = std::min( InSeconds, 20 );
  vector<int> last_build;

  // simulation time from now till InSeconds
  for( int i = 1 ; i <= min_time ; ++i )
  {
    for( const auto &t : current_state.inMove )
      if( t.action.creator.compare("Protoss_Probe") == 0
	  &&
	  t.action.name.compare("Mineral") == 0
	  &&
	  t.waitTime - i == 0 )
      {
	++workers;
      }
      
    for( const auto &t : current_state.busy )
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
  
double MinSpan::sharpGasIn( int duration, int InSeconds ) const
{
  double futurProduction = 0.;
  int workers = current_state.gasWorkers;

  // simulation time from now till InSeconds
  for( int i = 1 ; i <= InSeconds ; ++i )
  {
    for( const auto &t : current_state.inMove )
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

// double MinSpan::v_postprocessSatisfaction( vector< Action > *vecVariables,
// 						       BuildOrderDomain *domain,
// 						       double &bestCost,
// 						       vector< Action > &bestSolution,
// 						       double sat_timeout ) const
// {
//   chrono::time_point<chrono::high_resolution_clock> startPostprocess = chrono::high_resolution_clock::now(); 
//   chrono::duration<double,micro> postprocesstimer(0);

//   double cost = v_cost( vecVariables, domain );
    
//   if( cost < bestCost )
//   {
//     bestBO = bo;
//     bestCost = cost;
//     copy( begin(*vecVariables), end(*vecVariables), begin(bestSolution) );
//   }

//   postprocesstimer = chrono::high_resolution_clock::now() - startPostprocess;
//   return postprocesstimer.count();
// }


double MinSpan::v_postprocessOptimization( vector< Action > *vecVariables,
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

void MinSpan::makeVecVariables( const pair<string, int> &input, vector<Action> &variables, map< string, pair<int, int> > &goals )
{
  Action action( actionOf[input.first] );
  goals.emplace( action.getFullName(), make_pair<int, int>( static_cast<int>( input.second ), 0 ) );
    
  makeVecVariables( action, variables, input.second );
}

void MinSpan::makeVecVariables( const Action &action, vector<Action> &variables, int count )
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
  
// /*******************/
// /* MakeSpanMinCost */
// /*******************/
// MakeSpanMinCost::MakeSpanMinCost() : MinSpan( "MakeSpanMinCost" ) { }
// MakeSpanMinCost::MakeSpanMinCost( const vector< pair<string, int> > &input, vector<Action> &variables )
//   : MinSpan( "MakeSpanMinCost", input, variables ) { }

  
// // double MakeSpanMinCost::v_postprocessOptimization( vector< Action > *vecVariables, BuildOrderDomain *domain, double &bestCost )
// // {

// //   return 0;
// // }

  
// /*******************/
// /* MakeSpanMaxProd */
// /*******************/
// MakeSpanMaxProd::MakeSpanMaxProd() : MinSpan( "MakeSpanMaxProd" ) { }
// MakeSpanMaxProd::MakeSpanMaxProd( const vector< pair<string, int> > &input, vector<Action> &variables )
//   : MinSpan( "MakeSpanMaxProd", input, variables ) { }

// // double MakeSpanMaxProd::v_postprocessOptimization( vector< Action > *vecVariables, BuildOrderDomain *domain, double &bestCost )
// // {

// //   return 0;
// // }

