#include <numeric>
#include <memory>

#include "builder_bo.hpp"
#include "constraint_dependency.hpp"
#include "objective_makespan.hpp"

BuilderBO::BuilderBO( const std::vector< std::pair<std::string, int> >& inputs, bool exhaustive_inputs )
	: ModelBuilder( true ),
	  _domain_size(0)
{
	for( const auto& input : _inputs )
		makeGoals( input, exhaustive_inputs );

	for( const auto& goal : _goals )
		_domain_size += goal.second.first;
}

void BuilderBO::declare_variables()
{
	for( const auto& goal : _goals )
		for( int i = 0 ; i < goal.second.first ; ++i )
			create_variable( 0, _domain_size, goal.first );

	// assign variables to their ID, since it is a permutation problem
	for( auto& v: variables )
		v.set_value( v.get_id() );
}

void BuilderBO::declare_constraints()
{
	constraints.emplace_back( make_shared<Dependency>( variables ) );
}

void BuilderBO::declare_objective()
{
	objective = make_shared<MinSpan>( variables, _goals );
	// objective = make_shared<MakeSpanMinCost>( variables, _goals );
	// objective = make_shared<MakeSpanMaxProd>( variables, _goals );
}

void BuilderBO::makeGoals( std::pair<std::string, int> &input, bool exhaustive_inputs )
{
	Action action = action_of[input.first];
	for( int i = 1; i < input.second; ++i )
		_goals.emplace( action.name, std::make_pair<int, int>( input.second, 0 ) );

	if( !exhaustive_inputs )
		rec_makeGoals( action, input.second );	
}

void BuilderBO::rec_makeGoals( const Action &action, int count )
{
	if( count > 0 )
	{
		for( const auto &dep : action.dependencies )
			if( dep.compare( "Protoss_High_Templar" ) == 0 || dep.compare( "Protoss_Dark_Templar" ) == 0 )
				rec_makeGoals( action_of[ dep ], 2 * count ); // Each (dark) archon needs 2 (dark) templars 
			else
				if( dep.compare( "Protoss_Nexus" ) != 0 // if the dependency is not a Nexus
				    && !_goals.contains( dep ) )
					rec_makeGoals( action_of[ dep ], 1 );

		if( action.cost_gas > 0 && !_goals.contains( std::string( "Protoss_Assimilator" ) ) )
			rec_makeGoals( action_of[ "Protoss_Assimilator" ], 1 );
	}
}
