#include <algorithm>

#include "constraint_dependency.hpp"
#include "actionMap.hpp"

Dependency::Dependency( const std::vector< ghost::Variable >& variables )
	: Constraint(variables)
{ }

double Dependency::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	double conflicts = 0.;
	_already_built.clear();
	bool no_conflicts;
    
	for( int i = 0; i < variables.size(); ++i )
	{
		auto& action = action_of[ variables[i]->get_name() ];
		
		if( action.actionType == special
		    || action.depedencies.empty()
		    || ( action.depedencies.size() == 1 && action.depedencies.at(0).compare("Protoss_Nexus") == 0 ) )
			continue;
		
		no_conflicts = true;

		for( auto& depedency: action.depedencies )
			if( !_already_built.contains( depedency ) )
			{
				++conflicts;
				no_conflicts = false;
			}

		// If my action needs gas, do I have a gas extraction building?
		if( action.cost_gas > 0 && !_already_built.contains( std::string( "Protoss_Assimilator" ) ) )
		{
			++conflicts;
			no_conflicts = false;
		}

		if( no_conflicts )
			_already_built.insert( variables[i]->get_name() );
	}
    
	return conflicts;
}
