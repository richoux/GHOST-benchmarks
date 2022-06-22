#pragma once

#include <vector>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

using namespace std;
using namespace ghost;

class LinearLeq : public Constraint
{
	int _rhs;
	
	double required_error( const vector<Variable*>& variables ) const override;

public:
	LinearLeq( const vector<int>& variables, int rhs );
	LinearLeq( vector<int>&& variables, int rhs );
};
