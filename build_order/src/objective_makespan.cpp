#include <algorithm>
#include <limits>
#include <cmath>
#include <chrono>
#include <ctime>
#include <numeric>
#include <iomanip>

#include "objective_makespan.hpp"
#include "actionMap.hpp"

MinSpan::MinSpan( const std::vector<ghost::Variable>& variables,
                  // const std::map<string, pair<int, int> > &goals,
                  bool exhaustive_inputs )
	: Minimize( variables, "MinSpan" ),
	  // _goals(goals),
	  _current_state(State()),
	  // _bo(std::vector<BO>()),
	  // _bestBO(_bo),
	  _number_variables(static_cast<int>(variables.size())),
	  _actions(std::vector<ActionData>(_number_variables)),
	  _exhaustive_inputs(exhaustive_inputs)
{ }

// void MinSpan::printBO() const
// {
// 	std::cout << "\n\n";
// 	for( const auto &b : _bestBO )
// 	{
// 		if( b.name.compare("Protoss_Probe") != 0 && b.name.compare("Protoss_Pylon") != 0 )
// 			std::cout << b.name
// 			          << ": start at " << b.start_time
// 			          << ", finish at " << b.completed_time << "\n";
// 	}
// 	std::cout << "\n";
// }
  
double MinSpan::required_cost( const std::vector< ghost::Variable* >& variables ) const
{
	for( auto& v : variables )
		_actions[v->get_value()] = action_of.at( v->get_name() );
	
	_current_state.reset();
	// _bo.clear();

	// // we consider we start with 0 units/buildings we are looking to produce
	// for( auto &g : _goals)
	// 	g.second.second = 0;

	int i = 0;
	while( ( i < _number_variables || !_current_state.busy.empty() ) && _current_state.seconds < 50000 ) // < 50000 to prevent infinite loop. BOs shouldn't span too long anyway.
	{
		++_current_state.seconds;

		// update mineral / gas stocks
		_current_state.stock_mineral += _current_state.mineral_workers * mineral_rate; // minRate mineral per worker per second in average
		_current_state.stock_gas += _current_state.gas_workers * gas_rate; // gasRate gas per worker per second in average

		// update busy list
		update_busy();

		// update in_move list
		update_in_move();

		if( i < _number_variables )
		{
			deal_with_workers();
	
			// build a pylon if I must, ie:
			// 1. if I am not currently making pylons
			// 2. if my supply cap cannot manage the next global unit production
			if( !making_pylons() || _current_state.number_pylons != 0 )
				you_must_construct_additional_pylons();

			// // can I produce units?
			// if( _actions[ i ].actionType != ActionType::unit && i < _number_variables - 1 )
			// 	produce_units_first( _actions[ i ], variables );
	
			// can I handle the current action?
			if( handle_action_to_do( _actions[ i ] ) )
				++i;
			// else // can I handle the next action?
			// {
			// 	// if( nextAction != vecVariables->end() )
			// 	if( i < _actions.size() -1 )
			// 	{
			// 		auto& next_action = _actions[i + 1];

			// 		// book resources for the current action
			// 		int mineral_cost = _actions[ i ].cost_mineral;
			// 		int gas_cost = _actions[ i ].cost_gas;
	    
			// 		_current_state.minerals_booked += mineral_cost;
			// 		_current_state.gas_booked += gas_cost;
			// 		if( can_handle_building( next_action ) || can_handle_not_building( next_action ) )
			// 		{
			// 			// cout << "Swap " << actionToDo->getFullName() << ":" << actionToDo->getValue()
			// 			// 	   << " with " << nextAction->getFullName() << ":" << nextAction->getValue() << endl;

			// 			// std::swap( _actions[ i ], next_action );
			// 			action_to_do.swap( next_action );
			// 			_current_state.minerals_booked -= mineral_cost;
			// 			_current_state.gas_booked -= gas_cost;
			// 			if( handle_action_to_do( _actions[ i ] ) )
			// 				++i;
			// 			else
			// 			{
			// 				std::cout << "This should never append.\n";
			// 				std::exit( 0 );
			// 			}	      
			// 		}
			// 		else
			// 		{
			// 			_current_state.minerals_booked -= mineral_cost;
			// 			_current_state.gas_booked -= gas_cost;
			// 		}
			// 	}
			// }
		}
	}
	if( _current_state.seconds >= 50000 )
		return std::numeric_limits<double>::max(); // no solutions found
	else
		return static_cast<double>( _current_state.seconds );
}

// TODO: call the solver several times with different number of buildings producing units.
// double MinSpan::expert_postprocess( const std::vector< ghost::Variable* > &variables, double best_cost ) const
// {
// 	// only used by postprocessingOptimization, to see if we can
// 	// shorten the makespan by making more production buildings,
// 	// like gateways for instance.
// 	ActionData action, creator;
// 	double real_time, simulated_time;
// 	int simulated_mineral, simulated_gas;
// 	int future_mineral, future_gas;
	  
// 	int to_produce;
// 	int creator_in_production;
// 	int total_number;
// 	for( const auto &g : goals )
// 	{
// 		action = action_of[ g.first ];
// 		if( action.actionType == ActionType::building )
// 			continue;

// 		if( action.name.compare("Protoss_Archon") == 0 || action.name.compare("Protoss_Dark_Archon") == 0 )
// 			continue;
	    
// 		creator = action_of[ action.creator ];

// 		creator_in_production =
// 			count_if( begin(_current_state.in_move),
// 			          end(_current_state.in_move),
// 			          [&creator](ActionPrep& t){return t.action.name.compare( creator.name ) == 0;})
// 			+ count_if( begin(_current_state.busy),
// 			            end(_current_state.busy),
// 			            [&creator](ActionData& a){return a.name.compare( creator.name ) == 0;});

// 		total_number = _current_state.resources[creator.name].first + creator_in_production;
	    
// 		if( total_number == 0 )
// 			continue;
	
// 		// test if we are faster after making an additional production building
// 		to_produce = g.second.first - g.second.second;
// 		real_time = 0.;
	    
// 		for( const auto &t : _current_state.busy )
// 			if( t.name.compare( action.name ) == 0 )
// 			{
// 				--to_produce;
// 				real_time += t.seconds_required;
// 			}

// 		if( to_produce <= 0 )
// 			continue;

// 		// real_time: estimation of time needed to produce goal g with current production building
// 		// simulated_time: same estimation if we have one additional production building

// 		simulated_time = real_time + creator.seconds_required;
// 		real_time += to_produce * action.seconds_required / ( _current_state.resources[creator.name].first + creator_in_production );
// 		simulated_time += to_produce * action.seconds_required / ( _current_state.resources[creator.name].first + creator_in_production + 1 );

// 		if( simulated_time > real_time )
// 			continue;

// 		// test if we have enough money for making an additional production building
// 		simulated_mineral = ( total_number + 1 ) * action.cost_mineral;
// 		simulated_gas = ( total_number + 1 ) * action.cost_gas;

// 		future_mineral = sharp_minerals_in( action.seconds_required, creator.seconds_required );
// 		future_gas = sharp_gas_in( action.seconds_required, creator.seconds_required );

// 		// if we can make this additional building, do it! 
// 		if( future_mineral >= simulated_mineral && future_gas >= simulated_gas
// 		    &&
// 		    ( creator.cost_mineral == 0 || _current_state.stock_mineral >= creator.cost_mineral + _current_state.minerals_booked - minerals_in( go_to_build ) )
// 		    &&
// 		    ( creator.cost_gas == 0 || _current_state.stock_gas >= creator.cost_gas + _current_state.gas_booked - gas_in( go_to_build ) ) 
// 		    &&
// 		    _current_state.mineral_workers + _current_state.gas_workers > 0
// 		    &&
// 		    _current_state.number_pylons > 0
// 			)
// 		{
// 			_current_state.minerals_booked += creator.cost_mineral;
// 			_current_state.gas_booked += creator.cost_gas;
	    
// 			actions.emplace_back( ActionData( creator ) );
// 			// Create a new variables vector, with a new variable at the end and all domains incremented
// 			int new_size = (int)variables.size() + 1;
// 			// Warning: loop until new_size - 1 for the current variables vector
// 			for( int i = 0; i < new_size - 1; ++i )
// 			{
// 				auto var = variables[ i ];
// 				variables[ i ] = Variable( var.get_name(), var.get_short_name(), -1, new_size );
// 				variables[ i ].set_value( var.get_value() + 1 );
// 			}
// 			variables.emplace_back( creator.name, creator.name, -1, new_size );
// 			variables[ new_size - 1 ].set_value( i );
	  
// 			_current_state.in_move.push_back( ActionPrep( creator, go_to_build, _current_state.in_move.size() ) );
// 			if( _current_state.mineral_workers > 0 )
// 				--_current_state.mineral_workers;
// 			else
// 				--_current_state.gas_workers;

// #ifndef NDEBUG
// 			string text = "Optimize " + creator.name + " at ";
// 			cout << std::left << setw(35) << text << setw(5) << _current_state.seconds
// 			     << "  m = " << setw(9) << _current_state.stock_mineral
// 			     << "  g = " << setw(8) << _current_state.stock_gas
// 			     << "  mb = " << setw(5) << _current_state.minerals_booked
// 			     << "  gb = " << setw(4) << _current_state.gas_booked
// 			     << "  mw = " << setw(3) << _current_state.mineral_workers
// 			     << "  gw = " << setw(3) << _current_state.gas_workers
// 			     << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
// #endif	    
// 		}	    
// 	}
// }

void MinSpan::update_busy() const
{
	for( auto &t : _current_state.busy )
	{
		int time = t.decrease_seconds();
		if( time == 0 )
		{
			// if creator is something else than a probe, signal it as available
			// (recall: a probe is directly available after starting a building warping).
			if( t.creator.compare("Protoss_Probe") != 0 )
			{
				++_current_state.resources[ t.creator ].second;
			}

			// if the action was producing a probe, send it gathering minerals
			if( t.name.compare("Protoss_Probe") == 0 )
				_current_state.in_move.push_back( ActionPrep( action_of["Protoss_Mineral"], from_base_to_minerals, _current_state.in_move.size() ) );
			else
			{
				if( t.name.compare("Protoss_Nexus") == 0 )
				{
					++_current_state.resources["Protoss_Nexus"].first;
					++_current_state.resources["Protoss_Nexus"].second;
					_current_state.supply_capacity += 9;
					++_current_state.number_bases;
				}
				else if( t.name.compare("Protoss_Pylon") == 0 )
				{
					_current_state.supply_capacity += 8;
					++_current_state.number_pylons;
				}
				else if( t.name.compare("Protoss_Assimilator") == 0 )
				{
					++_current_state.number_refineries;
	    
					// if we have few workers mining, do not sent them to gas
					for( int i = 0; i < std::min( 3, _current_state.mineral_workers - 3 ); ++i )
					{
						_current_state.in_move.push_back( ActionPrep( action_of["Protoss_Gas"], from_minerals_to_gas, _current_state.in_move.size() ) );
						--_current_state.mineral_workers;
					}
				}
				else if( t.actionType == ActionType::building )
				{
					++_current_state.resources[ t.name ].first;
					++_current_state.resources[ t.name ].second;
				}
				else if( t.name.compare("Protoss_High_Templar") == 0
				         || t.name.compare("Protoss_Dark_Templar") == 0 )
				{
					++_current_state.resources[ t.name ].first;
					++_current_state.resources[ t.name ].second;
				}
			}

#ifndef NDEBUG
			std::string text = "Finish " + t.name + " at ";
			std::cout << std::left << std::setw(35) << text << std::setw(5) << _current_state.seconds
			          << "  m = " << std::setw(9) << _current_state.stock_mineral
			          << "  g = " << std::setw(8) << _current_state.stock_gas
			          << "  mb = " << std::setw(5) << _current_state.minerals_booked
			          << "  gb = " << std::setw(4) << _current_state.gas_booked
			          << "  mw = " << std::setw(3) << _current_state.mineral_workers
			          << "  gw = " << std::setw(3) << _current_state.gas_workers
			          << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
#endif
		}
	}

	auto itEnd = remove_if( begin( _current_state.busy ), end( _current_state.busy ), [](ActionData &a){return a.seconds_required == 0;} );
	_current_state.busy.erase( itEnd, end( _current_state.busy ) );
}
  
void MinSpan::update_in_move() const
{
	// (Me in 2014) The code below is not groovy, but it is written that way to avoid a really weird bug
	// (Me in 2018, reading the comment above) Dude, seriously...
	// (Me in 2026) Bros, this code gives me headaches.
	for_each( begin( _current_state.in_move ), end( _current_state.in_move ), [](ActionPrep &a){ if(a.wait_time > 0) --a.wait_time; } );
	auto copy_in_move(_current_state.in_move);
    
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
		    && ( copy_in_move[i].action.cost_mineral == 0 || _current_state.stock_mineral >= copy_in_move[i].action.cost_mineral )
		    && ( copy_in_move[i].action.cost_gas == 0 || _current_state.stock_gas >= copy_in_move[i].action.cost_gas )
			)
		{
			std::string creator = copy_in_move[i].action.creator;
			std::string goal = copy_in_move[i].action.name;

			int mineral_cost = copy_in_move[i].action.cost_mineral;
			int gas_cost = copy_in_move[i].action.cost_gas;

			if( creator.compare("Protoss_Probe") == 0 )
			{
				// The action is about to be done, so erase it from in_move
				auto to_erase = std::find_if( begin( _current_state.in_move ), end( _current_state.in_move ),
				                              [&](ActionPrep &a){ return copy_in_move[i].id == a.id; });
				for_each( to_erase, _current_state.in_move.end(), [](ActionPrep &a){ --a.id; } );
				for_each( begin( copy_in_move ) + i, end( copy_in_move ), [](ActionPrep &a){ --a.id; } );
				_current_state.in_move.erase( to_erase );
	  
				if( goal.compare("Mineral") == 0 ) 
					++_current_state.mineral_workers;
				else if( goal.compare("Gas") == 0 ) 
					++_current_state.gas_workers;
				else // ie, the worker is about to build something
				{
					push_in_busy( goal );
					// warp building and return to mineral fields
					_current_state.in_move.push_back( ActionPrep( action_of["Protoss_Mineral"], return_to_minerals, _current_state.in_move.size() ) );
	    
					_current_state.stock_mineral -= mineral_cost;
					_current_state.stock_gas -= gas_cost;
	    
					_current_state.minerals_booked -= mineral_cost;
					_current_state.gas_booked -= gas_cost;
	    
#ifndef NDEBUG
					std::string text = "Start " + goal + " at ";
					std::cout << std::left << std::setw(35) << text << std::setw(5) << _current_state.seconds
					          << "  m = " << std::setw(9) << _current_state.stock_mineral
					          << "  g = " << std::setw(8) << _current_state.stock_gas
					          << "  mb = " << std::setw(5) << _current_state.minerals_booked
					          << "  gb = " << std::setw(4) << _current_state.gas_booked
					          << "  mw = " << std::setw(3) << _current_state.mineral_workers
					          << "  gw = " << std::setw(3) << _current_state.gas_workers
					          << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
#endif
				} 
			}
		}
	}    
}

void MinSpan::deal_with_workers() const
{
	// send workers to gas, if need and possible
	if( _current_state.gas_workers + count_if( begin(_current_state.in_move),
	                                          end(_current_state.in_move),
	                                          [](ActionPrep &t){return t.action.name.compare("Gas") == 0;} )
	    < _current_state.number_refineries * 3 )
	{
		// if we have few workers mining, do not sent them to gas
		for( int i = 0 ; i < std::min( 3, _current_state.mineral_workers - 3 ) ; ++i )
		{
			_current_state.in_move.push_back( ActionPrep( action_of["Protoss_Gas"], from_minerals_to_gas, _current_state.in_move.size() ) );
			--_current_state.mineral_workers;
		}
	}
    
	// produce a worker if I can, ie:
	// 1. if I have at least 50 minerals
	// 2. if I have at least one available Nexus
	// 3. if I am not supply blocked
	// 4. if I don't reach the saturation number (ie 24 workers per base)
	if( _current_state.stock_mineral >= 50
	    &&
	    _current_state.resources["Protoss_Nexus"].second > 0
	    &&
	    _current_state.supply_used < _current_state.supply_capacity
	    &&
	    _current_state.mineral_workers + count_if( begin(_current_state.in_move),
	                                              end(_current_state.in_move),
	                                              [](const ActionPrep &t){return t.action.creator.compare("Protoss_Probe") == 0;})
	    < _current_state.number_bases * 24 )
	{
		_current_state.stock_mineral -= 50;
		++_current_state.supply_used;
		--_current_state.resources["Protoss_Nexus"].second;
		push_in_busy( "Protoss_Probe" );
      
#ifndef NDEBUG
		std::cout << std::left << std::setw(35) << "Start Protoss_Probe at " << std::setw(5) << _current_state.seconds
		          << "  m = " << std::setw(9) << _current_state.stock_mineral
		          << "  g = " << std::setw(8) << _current_state.stock_gas
		          << "  mb = " << std::setw(5) << _current_state.minerals_booked
		          << "  gb = " << std::setw(4) << _current_state.gas_booked
		          << "  mw = " << std::setw(3) << _current_state.mineral_workers
		          << "  gw = " << std::setw(3) << _current_state.gas_workers
		          << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
#endif
	}
}

// void MinSpan::produce_units_first( const ActionData &action_to_do, const std::vector< ghost::Variable* > &variables ) const
// {
// 	for( auto it = action_to_do + 1; it != variables.end(); ++it )
// 	{
// 		if( it->type == ActionType::unit && can_handle_not_building( *it ) )
// 		{
// 			// do a kind of reverse bubble-sort from it until action_to_do 
// 			for( auto it_swap = it - 1; it_swap != variables.begin() && it_swap != action_to_do - 1; --it_swap )
// 			{
// 				auto next = it_swap + 1;
// 				std::swap( *it_swap, *next );
// 			}
	
// 			if( handle_action_to_do( *action_to_do ) )
// 				++action_to_do;
// 			else
// 			{
// 				cout << "This should never append too.\n";
// 				exit( 0 );
// 			}	
// 		}
// 	}
// }
  
bool MinSpan::can_handle_building( const ActionData &action_to_do ) const
{
	if( action_to_do.actionType != ActionType::building )
		return false;

	// special case for the Assimilator
	// if( actionToDo.getFullName().compare("Protoss_Assimilator") == 0 )
	// {
      
	//   if(_current_state.seconds > 700)
	//   {
	//   	bool plop = find_if( begin(_current_state.busy),
	//   			     end(_current_state.busy),
	//   			     [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} ) != _current_state.busy.end();
	
	//   	cout << "nb ref: " << _current_state.numberRefineries
	//   	     << ", nb base:" << _current_state.numberBases
	//   	     << ", under build: " << plop << endl;
	//   }
      
	//   if( _current_state.stockMineral >= 100 + _current_state.mineralsBooked - mineralsIn(goToBuild)
	// 	  &&
	// 	  _current_state.mineralWorkers + _current_state.gasWorkers > 0
	// 	  &&
	// 	  _current_state.numberPylons > 0
	// 	  &&
	// 	  ( _current_state.numberRefineries < _current_state.numberBases
	// 	    || ( _current_state.numberRefineries + 1 == _current_state.numberBases
	// 		 && find_if( begin(_current_state.busy),
	// 			     end(_current_state.busy),
	// 			     [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} ) != _current_state.busy.end() )
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
	// if(_current_state.seconds > 700)
	// {
	// 	cout << "dep ok: " << dependenciesCheck( actionToDo.getFullName() ) << endl;
	// }

	if( ( action_to_do.cost_mineral == 0 || _current_state.stock_mineral >= action_to_do.cost_mineral + _current_state.minerals_booked - minerals_in( go_to_build ) )
	    &&
	    ( action_to_do.cost_gas == 0 || _current_state.stock_gas >= action_to_do.cost_gas + _current_state.gas_booked - gas_in( go_to_build ) ) 
	    &&
	    _current_state.mineral_workers + _current_state.gas_workers > 0
	    &&
	    _current_state.number_pylons > 0
	    &&
	    dependencies_check( action_to_do.name )
		)
	{
		return true;
	}
	else
		return false;
	// }
}
  
bool MinSpan::can_handle_not_building( const ActionData& action_to_do ) const
{
	if( action_to_do.actionType == ActionType::building )
		return false;

	if( ( action_to_do.cost_mineral == 0 || _current_state.stock_mineral >= action_to_do.cost_mineral + _current_state.minerals_booked )
	    &&
	    ( action_to_do.cost_gas == 0 || _current_state.stock_gas >= action_to_do.cost_gas + _current_state.gas_booked )
	    &&
	    _current_state.supply_used + action_to_do.cost_supply <= _current_state.supply_capacity
	    &&
	    ( action_to_do.creator.empty() || _current_state.resources[ action_to_do.creator ].second > 0 )
	    &&
	    dependencies_check( action_to_do.name )
		)
	{
		return true;
	}
	else
		return false;
}

bool MinSpan::handle_action_to_do( const ActionData& action_to_do ) const
{
	// if the next action is building a building
	if( action_to_do.actionType == ActionType::building )
	{
		if( can_handle_building( action_to_do ) )
		{
			_current_state.minerals_booked += action_to_do.cost_mineral;
			_current_state.gas_booked += action_to_do.cost_gas;
		
			_current_state.in_move.push_back( ActionPrep( action_to_do, go_to_build, _current_state.in_move.size() ) );
			if( _current_state.mineral_workers > 0 )
				--_current_state.mineral_workers;
			else
				--_current_state.gas_workers;

#ifndef NDEBUG
			std::string text = "Go for " + action_to_do.name + " at ";
			std::cout << std::left << std::setw(35) << text << std::setw(5) << _current_state.seconds
			          << "  m = " << std::setw(9) << _current_state.stock_mineral
			          << "  g = " << std::setw(8) << _current_state.stock_gas
			          << "  mb = " << std::setw(5) << _current_state.minerals_booked
			          << "  gb = " << std::setw(4) << _current_state.gas_booked
			          << "  mw = " << std::setw(3) << _current_state.mineral_workers
			          << "  gw = " << std::setw(3) << _current_state.gas_workers
			          << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
#endif
	
			return true;
		}
	}
	// otherwise, it is a unit/research/upgrade
	else
	{
		if( can_handle_not_building( action_to_do ) )
		{
			_current_state.supply_used += action_to_do.cost_supply;
			_current_state.stock_mineral -= action_to_do.cost_mineral;
			_current_state.stock_gas -= action_to_do.cost_gas;
	
			if( !action_to_do.creator.empty() && action_to_do.creator.compare("Protoss_Probe") != 0 )
				--_current_state.resources[ action_to_do.creator ].second;
	
			push_in_busy( action_to_do.name );
	
#ifndef NDEBUG
			std::string text = "Start " + action_to_do.name + " at ";
			std::cout << std::left << std::setw(35) << text << std::setw(5) << _current_state.seconds
			          << "  m = " << std::setw(9) << _current_state.stock_mineral
			          << "  g = " << std::setw(8) << _current_state.stock_gas
			          << "  mb = " << std::setw(5) << _current_state.minerals_booked
			          << "  gb = " << std::setw(4) << _current_state.gas_booked
			          << "  mw = " << std::setw(3) << _current_state.mineral_workers
			          << "  gw = " << std::setw(3) << _current_state.gas_workers
			          << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
#endif
	
			return true;
		}
	}

	return false;
}
  
bool MinSpan::making_pylons() const
{
	for( const auto &t : _current_state.busy )
		if( t.name.compare("Protoss_Pylon") == 0 )
			return true;

	for( const auto &t : _current_state.in_move )
		if( t.action.name.compare("Protoss_Pylon") == 0 )
			return true;    

	return false;
}

void MinSpan::you_must_construct_additional_pylons() const
{
	// build the first pylon ASAP
	if( _current_state.number_pylons == 0 )
	{
		if( _current_state.stock_mineral >= 100 - minerals_in( return_to_minerals ) )
		{
			_current_state.in_move.push_back( ActionPrep( action_of["Protoss_Pylon"], go_to_build, _current_state.in_move.size() ) );
	
			_current_state.minerals_booked += 100;
	
			if( _current_state.mineral_workers > 0 )
				--_current_state.mineral_workers;
			else
				--_current_state.gas_workers;

#ifndef NDEBUG
			std::cout << std::left << std::setw(35) << "Go for first Protoss_Pylon at " << std::setw(5) << _current_state.seconds
			          << "  m = " << std::setw(9) << _current_state.stock_mineral
			          << "  g = " << std::setw(8) << _current_state.stock_gas
			          << "  mb = " << std::setw(5) << _current_state.minerals_booked
			          << "  gb = " << std::setw(4) << _current_state.gas_booked
			          << "  mw = " << std::setw(3) << _current_state.mineral_workers
			          << "  gw = " << std::setw(3) << _current_state.gas_workers
			          << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
#endif	
		}
	}
	// otherwise build other pylons when needed
	else
	{
		int production_capacity =
			_current_state.resources["Protoss_Nexus"].first
			+ count_if( begin(_current_state.busy), end(_current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;})

			+ 2 * ( _current_state.resources["Protoss_Gateway"].first
			        + count_if( begin(_current_state.busy), end(_current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Gateway" ) == 0;}) )
			+ 4 * ( _current_state.resources["Protoss_Robotics_Facility"].first
			        + count_if( begin(_current_state.busy), end(_current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Robotics_Facility" ) == 0;}) )
			+ 6 * ( _current_state.resources["Protoss_Stargate"].first
			        + count_if( begin(_current_state.busy), end(_current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Stargate" ) == 0;}) );

		int planned_supply = _current_state.supply_capacity
			+ 8 * count_if( begin(_current_state.busy), end(_current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Pylon" ) == 0;} )
			+ 8 * count_if( begin(_current_state.in_move), end(_current_state.in_move), [](ActionPrep &t){return t.action.name.compare("Protoss_Pylon") == 0;} )
			+ 9 * count_if( begin(_current_state.busy), end(_current_state.busy), [](ActionData &a){return a.name.compare( "Protoss_Nexus" ) == 0;} )
			+ 9 * count_if( begin(_current_state.in_move), end(_current_state.in_move), [](ActionPrep &t){return t.action.name.compare("Protoss_Nexus") == 0;} );
    
		if( planned_supply <= production_capacity + _current_state.supply_used )
		{
			int count_build = 0;
			do
			{
				_current_state.in_move.push_back( ActionPrep( action_of["Protoss_Pylon"], go_to_build, _current_state.in_move.size() ) );
				_current_state.minerals_booked += 100;
				++count_build;
			} while( planned_supply + ( 8 * count_build ) <= production_capacity + _current_state.supply_used );

			// The same worker will build pylons. Several go_to_build/return_to_minerals seconds would be counted, but this is not a big deal.
			if( _current_state.mineral_workers > 0 )
				--_current_state.mineral_workers;
			else
				--_current_state.gas_workers;

#ifndef NDEBUG
			std::cout << std::left << std::setw(35) << "Go for Protoss_Pylon at " << std::setw(5) << _current_state.seconds
			          << "  m = " << std::setw(9) << _current_state.stock_mineral
			          << "  g = " << std::setw(8) << _current_state.stock_gas
			          << "  mb = " << std::setw(5) << _current_state.minerals_booked
			          << "  gb = " << std::setw(4) << _current_state.gas_booked
			          << "  mw = " << std::setw(3) << _current_state.mineral_workers
			          << "  gw = " << std::setw(3) << _current_state.gas_workers
			          << "  s = " << _current_state.supply_used << "/" << _current_state.supply_capacity << ")\n";
#endif
		}
	}
}

void MinSpan::push_in_busy( const std::string& name ) const
{
	ActionData a  = action_of[ name ];
	_current_state.busy.push_back( a );
    
	//bo.emplace_back( a.name, _current_state.seconds, _current_state.seconds + a.secondsRequired );
	// if( goals.find( a.name ) != goals.end() )
	// 	++goals.at( a.name ).second;
}


// TOCHECK: are upgrades like Protoss_Ground_Weapons_X written in state.resources or not?
// If not, I don't see where do we check we got Protoss_Ground_Weapons_1 before accepting Protoss_Ground_Weapons_2
bool MinSpan::dependencies_check( const std::string& s ) const
{
	ActionData data = action_of[ s ]; 
	if( data.cost_gas > 0 && _current_state.number_refineries == 0 )
		return false;

	// Ok, this if statement is a bit tricky
	// return false if, for any dependency: 
	if( any_of( begin( data.dependencies ), end( data.dependencies ),
	            [&](const std::string &n)
	            {
		            // 1. the dependency has not been created yet
		            return ( _current_state.resources[ n ].first == 0
		                     &&
		                     // 2. and (it is not planned to get one before go_to_build seconds OR data not a building)
		                     ( data.actionType != ActionType::building
		                       ||
		                       none_of( begin( _current_state.busy ), end( _current_state.busy ),
		                                [&n](ActionData &a)
		                                {
			                                return ( a.name.compare( n ) == 0 && a.seconds_required <= go_to_build );
		                                })
			                     ) 
			            );
	            } ) )
		return false;

	return true;
}

/////////////////////////
/////////////////////////
/////////////////////////
/////////////////////////
double MinSpan::sharp_minerals_in( int duration, int in_seconds ) const
{
	double futur_production = 0.;
	int workers = _current_state.mineral_workers;

	int min_time = std::min( in_seconds, 20 );
	std::vector<int> last_build;

	// simulation time from now till in_seconds
	for( int i = 1 ; i <= min_time ; ++i )
	{
		// NB: costly. Can be improved by counting how much t occurs within min_time
		for( const auto &t : _current_state.in_move )
			if( t.action.creator.compare("Protoss_Probe") == 0
			    &&
			    t.action.name.compare("Mineral") == 0
			    &&
			    t.wait_time - i == 0 )
			{
				++workers;
			}
      
		for( const auto &t : _current_state.busy )
			if( t.name.compare("Protoss_Probe") == 0
			    &&
			    t.seconds_required + 2 - i == 0 )
			{
				++workers;
				last_build.push_back( i );
			}
	}

	for( int i = min_time + 1; i <= in_seconds; ++i )
	{
		for( const auto &l : last_build )
			if( ( i + 2 - l ) % 20 == 0 )
				++workers;
	}

	// start to count income from in_seconds till in_seconds + duration
	for( int i = in_seconds + 1; i <= in_seconds + duration; ++i )
	{
		for( const auto &l : last_build )
			if( ( i + 2 - l ) % 20 == 0 )
				++workers;
      
		futur_production += workers * mineral_rate;
	}
    
	return futur_production;
}
  
double MinSpan::sharp_gas_in( int duration, int in_seconds ) const
{
	double futur_production = 0.;
	int workers = _current_state.gas_workers;

	// simulation time from now till in_seconds
	for( int i = 1 ; i <= in_seconds ; ++i )
	{
		for( const auto &t : _current_state.in_move )
			if( t.action.creator.compare("Protoss_Probe") == 0
			    &&
			    t.action.name.compare("Gas") == 0
			    &&
			    t.wait_time - i == 0 )
			{
				++workers;
			}
	}

	// start to count income from in_seconds till in_seconds + duration
	for( int i = in_seconds + 1 ; i <= in_seconds + duration ; ++i )
		futur_production += workers * mineral_rate;

	return futur_production;
}
  
// /*******************/
// /* MakeSpanMinCost */
// /*******************/
// MakeSpanMinCost::MakeSpanMinCost() : MinSpan( "MakeSpanMinCost" ) { }
// MakeSpanMinCost::MakeSpanMinCost( const vector< pair<string, int> > &input, vector<Action> &variables )
//   : MinSpan( "MakeSpanMinCost", input, variables ) { }

  
// // double MakeSpanMinCost::v_postprocessOptimization( vector< Action > *variables, BuildOrderDomain *domain, double &bestCost )
// // {

// //   return 0;
// // }

  
// /*******************/
// /* MakeSpanMaxProd */
// /*******************/
// MakeSpanMaxProd::MakeSpanMaxProd() : MinSpan( "MakeSpanMaxProd" ) { }
// MakeSpanMaxProd::MakeSpanMaxProd( const vector< pair<string, int> > &input, vector<Action> &variables )
//   : MinSpan( "MakeSpanMaxProd", input, variables ) { }

// // double MakeSpanMaxProd::v_postprocessOptimization( vector< Action > *variables, BuildOrderDomain *domain, double &bestCost )
// // {

// //   return 0;
// // }

