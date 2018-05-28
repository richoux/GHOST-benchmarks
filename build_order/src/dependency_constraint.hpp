#pragma once

#include <vector>
#include <functional>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

#include "actionData.hpp"
#include "actionMap.hpp"

using namespace std;
using namespace ghost;

class DependencyConstraint : public Constraint
{
  const vector< ActionData >& actions;
  
  double required_cost() const override;

public:
  DependencyConstraint( const vector< reference_wrapper<Variable> >& variables,
			const vector< ActionData >& actions );
};  
