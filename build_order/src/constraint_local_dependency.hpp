#pragma once

#include <vector>
#include <set>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

class Dependency : public ghost::Constraint
{
  double required_error( const std::vector<ghost::Variable*>& variables ) const override;

public:
	Dependency( const std::vector< int >& variables_index );
};  
