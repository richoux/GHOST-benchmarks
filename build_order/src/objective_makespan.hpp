#pragma once

#include <vector>
#include <map>
#include <memory>

#include <ghost/variable.hpp>
#include <ghost/objective.hpp>

#include "actionData.hpp"
#include "actionPrep.hpp"
#include "state.hpp"
    
class MinSpan : public ghost::Minimize
{
	struct BO
	{
		BO( std::string full_name, int start_time, int completed_time )
			: full_name(full_name), start_time(start_time), completed_time(completed_time) { }
		
		std::string full_name;
		int start_time;
		int completed_time;
	};

	// <goal name, [number to produce, current number]>
	// <Gateway, [2,1]> means we have currently one Gateway (possibly under construction) and we aim to have two of these.
	// mutable std::map< string, pair<int, int> > _goals;
	mutable State	_current_state;
	// mutable std::vector<BO> _bo;
	// mutable std::vector<BO> _bestBO;

	int _number_variables;
	mutable std::vector< ActionData > _actions;
	bool _exhaustive_inputs;
	
	double required_cost( const std::vector< ghost::Variable* >& variables ) const override;
	// double expert_postprocess( const std::vector< ghost::Variable* > &variables, double best_cost ) const override;
	
	void update_busy() const;
	void update_in_move() const;
	void deal_with_workers() const;
	bool can_handle_building( const ActionData& ) const;
	bool can_handle_not_building( const ActionData& ) const;
	bool handle_action_to_do( const ActionData& ) const;
	// void produce_units_first( const ActionData&, const std::vector<ghost::Variable*>& ) const;
	bool making_pylons() const;
	void you_must_construct_additional_pylons() const;
	void push_in_busy( const std::string& ) const;
	bool dependencies_check( const std::string& ) const;
    
	// rough estimations
	inline double minerals_in( int duration )	const { return _current_state.mineral_workers * mineral_rate * duration; }
	inline double gas_in( int duration ) const { return _current_state.gas_workers * gas_rate * duration; }

	// sharp estimations
	double sharp_minerals_in( int duration, int in_seconds = 0 ) const;
	double sharp_gas_in( int duration, int in_seconds = 0 ) const;
	
public:
	MinSpan( const std::vector<ghost::Variable>& variables,
	         // const std::map<string, pair<int, int> > &goals,
	         bool exhaustive_inputs );

	// void printBO() const;
};
  
// /*******************/
// /* MakeSpanMinCost */
// /*******************/
// class MakeSpanMinCost : public MinSpan
// {
// public:
//   MakeSpanMinCost();
//   MakeSpanMinCost( const vector< pair<string, int> > &input, vector<Action> &variables );

//   // private:
//   //   double v_postprocessOptimization( vector< Action > *vecVariables, BuildOrderDomain *domain, double &bestCost );
// };

// /*******************/
// /* MakeSpanMaxProd */
// /*******************/
// class MakeSpanMaxProd : public MinSpan
// {
// public:
//   MakeSpanMaxProd();
//   MakeSpanMaxProd( const vector< pair<string, int> > &input, vector<Action> &variables );

//   // private:
//   //   double v_postprocessOptimization( vector< Action > *vecVariables, BuildOrderDomain *domain, double &bestCost );
// };
