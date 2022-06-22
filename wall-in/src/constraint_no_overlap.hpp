#pragma once

#include <vector>
#include <ghost/constraint.hpp>

#include "placement.hpp"
#include "building.hpp"

using namespace std;
using namespace ghost;


class NoOverlap : public Constraint
{
	friend class CoverStartAndTarget;

	Placement _placement;
	vector<Building> _buildings;
	int _width;
	int _height;
	mutable int _error;
	
public:
	NoOverlap( const vector<Variable>& variables,
	           const vector<Building>& buildings,
	           int width,
	           int height );

	double required_error( const vector<Variable>& variables ) const override;

  double expert_delta_error( const std::vector<Variable>& variables,
	                           const std::vector<unsigned int>& variable_indexes,
	                           const std::vector<int>& candidate_values ) const override;

	void update_constraint( const std::vector<Variable>& variables,
	                        unsigned int variable_index,
	                        int new_value );
};
