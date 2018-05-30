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
  currentState.reset();
  bo.clear();
  for( auto &g : goals)
    g.second.second = 0;

  // auto action_to_do = actions.begin();
    
  // while( action_to_do != action.end() || !current_state.busy.empty() )
  int i = 0;
  while( i < actions.size() || !current_state.busy.empty() )
  {
    auto& action_to_do = actions[i];
    // cout << "vec size: " << vecVariables->size()
    // 	   << ", iter: " << std::distance( actionToDo, vecVariables->end() )
    // 	   << ", busy: " << currentState.busy.size() << endl;

    // if( currentState.seconds > 700 )
    // {
    // 	for( auto itit = actionToDo ; itit != vecVariables->end() ; ++itit )
    // 	  cout << itit->getFullName() << "(" << canHandleNotBuilding( *itit ) << ") ";
    // 	cout << endl;	
    // }
      
    ++current_state.seconds;

    // update mineral / gas stocks
    current_state.stock_mineral += current_state.mineral_workers * min_rate; // minRate mineral per worker per second in average
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
  // while( actionToDo != copyVec.end() || !currentState.busy.empty() )
  while( i < copyVec.size() || !current_state.busy.empty() )
  {
    ++current_state.seconds;

    // update mineral / gas stocks
    current_state.stock_mineral += current_state.mineral_workers * min_rate;
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
	real_time += to_produce * action.seconds_required / ( currentState.resources[creator.name].first + creator_in_production );
	simulated_time += to_produce * action.seconds_required / ( currentState.resources[creator.name].first + creator_in_production + 1 );

	// real_time: estimation of time needed to produce goal g with current production building
	// simulated_time: same estimation if we have one additional production building
	
	if( simulated_time > real_time )
	  continue;

	// test is we have enough money for making an additional production building
	simulated_mineral = ( currentState.resources[creator.name].first + creator_in_production + 1 ) * action.costMineral;
	simulated_gas = ( currentState.resources[creator.name].first + creator_in_production + 1 ) * action.costGas;

	future_mineral = sharp_minerals_in( action.seconds_required, creator.seconds_required );
	future_gas = sharp_gas_in( action.seconds_required, creator.seconds_required );

	// if we can make this additional building, do it! 
	if( future_mineral >= simulated_mineral && future_gas >= simulated_gas
	    &&
	    ( creator.cost_mineral == 0 || currentState.stock_mineral >= creator.cost_mineral + currentState.minerals_booked - minerals_in( go_to_build ) )
	    &&
	    ( creator.cost_gas == 0 || currentState.stock_gas >= creator.cost_gas + currentState.gas_booked - gas_in( go_to_build ) ) 
	    &&
	    currentState.mineral_workers + currentState.gas_workers > 0
	    &&
	    currentState.number_pylons > 0
	    )
	{
	  currentState.minerals_booked += creator.cost_mineral;
	  currentState.gas_booked += creator.cost_gas;
	    
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
	  
	  currentState.in_move.push_back( ActionPrep( creator, go_to_build, currentState.in_move.size() ) );
	  if( currentState.mineral_workers > 0 )
	    --currentState.mineral_workers;
	  else
	    --currentState.gas_workers;

#ifndef NDEBUG
	  string text = "Optimize " + creator.name + " at ";
	  cout << std::left << setw(35) << text << setw(5) << currentState.seconds
	       << "  m = " << setw(9) << currentState.stock_mineral
	       << "  g = " << setw(8) << currentState.stock_gas
	       << "  mb = " << setw(5) << currentState.minerals_booked
	       << "  gb = " << setw(4) << currentState.gas_booked
	       << "  mw = " << setw(3) << currentState.mineral_workers
	       << "  gw = " << setw(3) << currentState.gas_workers
	       << "  s = " << currentState.supply_used << "/" << currentState.supply_capacity << ")" << endl;
#endif	    
	}	    
      }	  
	
      // build a pylon if I must, ie:
      // 1. if I am not currently making pylons
      // 2. if my supply cap cannot manage the next global unit production
      if( !making_pylons() || currentState.number_pylons != 0 )
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
	    
	  currentState.minerals_booked += mineral_cost;
	  currentState.gas_booked += gas_cost;
	  if( can_handle_building( actions[ i + 1 ] ) || can_handle_not_building( actions[ i + 1 ] ) )
	  {
	    // cout << "Swap " << actionToDo->getFullName() << ":" << actionToDo->getValue()
	    // 	   << " with " << nextAction->getFullName() << ":" << nextAction->getValue() << endl;

	    std::swap( actions[ i ], actions[ i + 1 ] );
	    currentState.minerals_booked -= mineral_cost;
	    currentState.gas_booked -= gas_cost;
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
	    currentState.minerals_booked -= mineral_cost;
	    currentState.gas_booked -= gas_cost;
	  }
	}
      }
    }
  }
  return static_cast<double>( currentState.seconds );
}

void MinSpan::update_busy() const
{
  for( auto &t : currentState.busy )
  {
    int time = t.decrease_seconds();
    if( time == 0 )
    {
      if( t.creator.compare("Protoss_Probe") != 0 )
      {
	++currentState.resources[ t.creator ].second;
      }
      if( t.name.compare("Protoss_Probe") == 0 )
	currentState.in_move.push_back( ActionPrep( action_of["Protoss_Mineral"], from_base_to_minerals, currentState.in_move.size() ) );
      else
      {
	if( t.name.compare("Protoss_Nexus") == 0 )
	{
	  ++currentState.resources["Protoss_Nexus"].first;
	  ++currentState.resources["Protoss_Nexus"].second;
	  currentState.supply_capacity += 9;
	  ++currentState.number_bases;
	}
	else if( t.name.compare("Protoss_Pylon") == 0 )
	{
	  currentState.supply_capacity += 8;
	  ++currentState.number_pylons;
	}
	else if( t.name.compare("Protoss_Assimilator") == 0 )
	{
	  ++currentState.number_refineries;
	    
	  // if we have few workers mining, do not sent them to gas
	  for( int i = 0; i < min( 3, currentState.mineral_workers - 3 ); ++i )
	  {
	    currentState.in_move.push_back( ActionPrep( action_of["Protoss_Gas"], from_min_to_gas, currentState.in_move.size() ) );
	    --currentState.mineral_workers;
	  }
	}
	else if( t.actionType == ActionType::building )
	{
	  ++currentState.resources[ t.name ].first;
	  ++currentState.resources[ t.name ].second;
	  currentState.in_move.push_back( ActionPrep( action_of["Protoss_Mineral"], return_to_minerals, currentState.in_move.size() ) );
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
	   << "  m = " << setw(9) << currentState.stock_mineral
	   << "  g = " << setw(8) << currentState.stock_gas
	   << "  mb = " << setw(5) << currentState.minerals_booked
	   << "  gb = " << setw(4) << currentState.gas_booked
	   << "  mw = " << setw(3) << currentState.mineral_workers
	   << "  gw = " << setw(3) << currentState.gas_workers
	   << "  s = " << currentState.supply_used << "/" << currentState.supply_capacity << ")" << endl;
#endif
    }
  }

  auto itEnd = remove_if( begin( currentState.busy ), end( currentState.busy ), [](ActionData &a){return a.seconds_required == 0;} );
  currentState.busy.erase( itEnd, end( currentState.busy ) );
}
  
void MinSpan::update_in_move() const
{
  // remove from the inMove list actions just done
  // auto itEnd = remove_if( begin( currentState.inMove ), end( currentState.inMove ), [](ActionPrep &t){return t.done;} );
  // currentState.inMove.erase( itEnd, end( currentState.inMove ) );

  // The code below is not groovy
  // but it is written that way to avoid a really weird bug
  for_each( begin( currentState.in_move ), end( currentState.in_move ), [](ActionPrep &a){ if(a.wait_time > 0) --a.wait_time; } );
  auto copyInMove(currentState.in_move);
    
  for( int i = 0 ; i < copy_in_move.size() ; ++i )
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
	&& ( copy_in_move[i].action.cost_mineral == 0 || currentState.stock_mineral >= copy_in_move[i].action.cost_mineral )
	&& ( copy_in_move[i].action.cost_gas == 0 || currentState.stock_gas >= copy_in_move[i].action.cost_gas )
	)
    {
      string creator = copy_in_move[i].action.creator;
      string goal = copy_in_move[i].action.name;

      int mineral_cost = copy_in_move[i].action.cost_mineral;
      int gas_cost = copy_in_move[i].action.cost_gas;

      if( creator.compare("Protoss_Probe") == 0 )
      {
	// The action is about to be done, so erase it from inMove
	auto to_erase = std::find_if( begin( currentState.in_move ), end( currentState.in_move ),
				     [&](ActionPrep &a){ return copy_in_move[i].id == a.id; });
	for_each( to_erase, currentState.in_move.end(), [](ActionPrep &a){ --a.id; } );
	for_each( begin( copy_in_move ) + i, end( copy_in_move ), [](ActionPrep &a){ --a.id; } );
	currentState.in_move.erase( to_erase );
	  
	if( goal.compare("Mineral") == 0 ) 
	  ++currentState.mineral_workers;
	else if( goal.compare("Gas") == 0 ) 
	  ++currentState.gas_workers;
	else // ie, the worker is about to build something
	{
	  push_in_busy( copy_in_move[i].action.name );
	  // warp building and return to mineral fields
	  currentState.in_move.push_back( ActionPrep( action_of["Protoss_Mineral"], return_to_minerals, currentState.in_move.size() ) );
	    
	  currentState.stock_mineral -= mineral_cost;
	  currentState.stock_gas -= gas_cost;
	    
	  currentState.minerals_booked -= mineral_cost;
	  currentState.gas_booked -= gas_cost;
	    
#ifndef NDEBUG
	  string text = "Start " + goal + " at ";
	  cout << std::left << setw(35) << text << setw(5) << currentState.seconds
	       << "  m = " << setw(9) << currentState.stock_mineral
	       << "  g = " << setw(8) << currentState.stock_gas
	       << "  mb = " << setw(5) << currentState.minerals_booked
	       << "  gb = " << setw(4) << currentState.gas_booked
	       << "  mw = " << setw(3) << currentState.mineral_workers
	       << "  gw = " << setw(3) << currentState.gas_workers
	       << "  s = " << currentState.supply_used << "/" << currentState.supply_capacity << ")" << endl;
#endif
	} 
      }
    }
  }    
}

void MinSpan::deal_with_workers() const
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

void MinSpan::produceUnitsFirst( vector<Action>::iterator &actionToDo, vector< Action > *vecVariables ) const
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
  
bool MinSpan::canHandleBuilding( const Action &actionToDo ) const
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
  
bool MinSpan::canHandleNotBuilding( const Action &actionToDo ) const
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

bool MinSpan::handleActionToDo( const Action &actionToDo ) const
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
  
bool MinSpan::makingPylons() const
{
  for( const auto &t : currentState.busy )
    if( t.name.compare("Protoss_Pylon") == 0 )
      return true;

  for( const auto &t : currentState.inMove )
    if( t.action.name.compare("Protoss_Pylon") == 0 )
      return true;    

  return false;
}

void MinSpan::youMustConstructAdditionalPylons() const
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

void MinSpan::pushInBusy( const string  &name ) const
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

bool MinSpan::dependenciesCheck( const string &s ) const
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

double MinSpan::sharpMineralsIn( int duration, int InSeconds ) const
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
  
double MinSpan::sharpGasIn( int duration, int InSeconds ) const
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

