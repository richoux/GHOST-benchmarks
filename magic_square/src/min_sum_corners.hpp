#pragma once

#include <ghost/objective.hpp>

using namespace std;
using namespace ghost;

class MinCorners : public Objective
{
	int _number_variables;
	int _instance_size;
	
public:
	MinCorners( const vector<Variable>& variables );
	double required_cost( const vector<Variable*>& variables ) const override;
};
