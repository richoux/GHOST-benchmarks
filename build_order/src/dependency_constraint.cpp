#include <algorithm>

#include "dependency_constraint.hpp"

DependencyConstraint::DependencyConstraint( const vector< reference_wrapper<Variable> >& variables,
					    const vector< ActionData >& actions )
  : Constraint(variables),
    actions(actions)
{ }


double DependencyConstraint::required_cost() const
{
  double conflicts = 0.;
  // bool depConflict;
    
  for( int i = 0; i < variables.size(); ++i )
  {
    if( actions[i].actionType == special )
      continue;
	
    // depConflict = false;
    auto& action = actions[i];
    auto& dep = actions[i].dependencies;
    auto it = actions.begin() + i;
    
    if( !dep.empty() && !( dep.size() == 1 && dep.at(0).compare("Protoss_Nexus") == 0 ) )
      for( const auto &d : dep )

	// Is a dependency not satisfied?
	if( none_of( actions.begin(), it, [&](ActionData &a){return d.compare(a.name) == 0;} ) )
	{
	  ++conflicts;
	  // depConflict = true;
	    
	  // for( auto it_dep = it+1 ; it_dep != actions.end() ; ++it_dep )
	  //   if( d.compare( it_dep->name ) == 0 )
	  //   {
	  //     conflicts += 2;
	  //   }
	}

    // If my action needs gas, do I have a gas extraction building?
    if( action.cost_gas > 0 && none_of( actions.begin(), it, [&](ActionData &a){return a.name.compare("Protoss_Assimilator") == 0;} ) )
    {
      ++conflicts;
      // depConflict = true;
	
      // for( auto it_dep = it+1 ; it_dep != actions.end() ; ++it_dep )
      // 	if( it_dep->name.compare("Protoss_Assimilator") == 0 )
      // 	{
      // 	  conflicts += 2;
      // 	}
    }

    // if( depConflict )
    // {
    //   conflicts += 3;
    // }
  }
    
  return conflicts;
}
