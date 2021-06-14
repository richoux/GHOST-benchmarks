#pragma once

#include <vector>
#include <functional>
#include <memory>

#include <ghost/constraint.hpp>
#include <ghost/variable.hpp>

#include "data.hpp"

using namespace std;
using namespace ghost;

enum ResourceType { Mineral, Gas, Supply };

class Stock : public Constraint
{
  int	_quantity;
  ResourceType _type;
	vector<UnitData> _unit_data;
	mutable double _current_diff;

	double required_error( const vector<Variable*>& variables ) const override;

	double expert_delta_error( const vector<Variable*>& variables,
	                           const vector<int>& variable_indexes,
	                           const vector<int>& candidate_values ) const override;

	void expert_update_if_delta_error_defined( const vector<Variable*>& variables, int variable_id, int new_value ) override;
public:
  Stock( const vector<Variable>& variables,
         int quantity,
         ResourceType type,
         shared_ptr<AuxiliaryData> auxiliary_data );
	
	inline int get_resource() { return _quantity; }
};
