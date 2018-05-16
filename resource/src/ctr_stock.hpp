#pragma once

#include <vector>
// #include <functional>

#include <ghost/constraint.hpp>
#include <ghost/variable.hpp>

#include "unitData.hpp"

using namespace std;
using namespace ghost;

enum ResourceType { Mineral, Gas, Supply };

class Stock : public Constraint
{
  int			_quantity;
  ResourceType		_type;
  vector< UnitData >	_unit_data;

  double required_cost() const override;

public:
  Stock( const vector< Variable* >& variables,
	 int quantity,
	 ResourceType type,
	 const vector< UnitData >& unit_data );
  // Stock( const vector< reference_wrapper<Variable> >& variables,
  // 	 int quantity,
  // 	 ResourceType type,
  // 	 const vector< UnitData >& unit_data );

  inline int get_resource() { return _quantity; }
};
