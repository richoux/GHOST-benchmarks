#pragma once

#include <vector>
#include <set>

#include <ghost/variable.hpp>
#include <ghost/constraint.hpp>

class Dependency : public ghost::Constraint
{
	std::set<std::string> _already_built;
	
  double required_error( const std::vector<ghost::Variable*>& variables ) const override;

public:
	Dependency( const std::vector< ghost::Variable >& variables );
};  
