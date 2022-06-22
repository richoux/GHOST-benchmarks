#pragma once

#include <vector>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

using namespace std;
using namespace ghost;

class LinearGeq : public Constraint
{
	int _rhs;
	
	double required_error( const vector<Variable*>& variables ) const override;

public:
	LinearGeq( const vector<int>& variables, int rhs );
	LinearGeq( vector<int>&& variables, int rhs );
};
