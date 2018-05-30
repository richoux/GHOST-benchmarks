#pragma once

#include <vector>
#include <map>
#include <memory>

#include <ghost/variable.hpp>
#include <ghost/objective.hpp>

#include "actionData.hpp"
#include "actionMap.hpp"
#include "actionPrep.hpp"
#include "state.hpp"
#include "bo.hpp"

using namespace std;
using namespace ghost;

// Commented value are values used by Churchill et al. in "Build Order Optimization in StarCraft".
// These values are too advantageous and then too far away from real values in StarCraft IMHO.
constexpr int go_to_build		= 5; //5
constexpr int return_to_minerals	= 4; //4
constexpr int from_base_to_minerals	= 5; //2
constexpr int from_min_to_gas		= 5; //2

constexpr double min_rate		= 0.68; //1.07
constexpr double gas_rate		= 1.15; //1.66
    
class MinSpan : public Objective
{
  double required_cost( const vector< Variable >& vecVariables ) const override;
  void expert_postprocess_optimization( vector< Variable >& variables,
					double& bestCost,
					vector<int>& solution )	const override;
  
  void update_busy() const;
  void update_in_move() const;
  void deal_with_workers() const;
  bool can_handle_building( const ActionData& ) const;
  bool can_handle_not_building( const ActionData& ) const;
  double cost_optimization( const vector< Variable >& vecVariables ) const
  bool handle_action_to_do( const ActionData& ) const;
  void produce_units_first( const ActionData&, const vector<Variable>& ) const;
  bool making_pylons() const;
  void you_must_construct_additional_pylons() const;
  void push_in_busy( const string& ) const;
  bool dependencies_check( const string& ) const;
    
  // rough estimations
  inline double minerals_in( int duration )	const { return current_state.mineral_workers * min_rate * duration; }
  inline double gas_in( int duration )		const { return current_state.gas_workers * gas_rate * duration; }

  // sharp estimations
  double sharp_minerals_in( int duration, int in_seconds = 0 ) const;
  double sharp_gas_in( int duration, int in_seconds = 0 ) const;

  mutable State				current_state;

  // <goal name, [number to produce, current number]>
  // <Gateway, [2,1]> means we have currently one Gateway (possibly under construction) and we aim to have two of these.
  mutable map< string, pair<int, int> >	goals;
  
  mutable vector<BO>			bo;
  mutable vector<BO>			bestBO;
  
  vector< ActionData >			actions;

public:
  MinSpan( const vector< ActionData >& actions );

  void printBO() const;
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
