#include <numeric>
#include <memory>

#include "builder_bo.hpp"
#include "constraint_dependency.hpp"
#include "objective_makespan.hpp"

BuilderBO::BuilderBO( const std::vector< std::pair<std::string, int> >& inputs, bool exhaustive_inputs )
	: ModelBuilder( true ),
	  _domain_size(0),
	  _exhaustive_inputs(exhaustive_inputs)
{
	for( const std::pair<std::string, int>& input : inputs )
		makeGoals( input.first, input.second );

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
	objective = make_shared<MinSpan>( variables, _exhaustive_inputs );
	// objective = make_shared<MakeSpanMinCost>( variables, _goals, _exhaustive_inputs );
	// objective = make_shared<MakeSpanMaxProd>( variables, _goals, _exhaustive_inputs );
}

void BuilderBO::makeGoals( std::string name, int number )
{
	ActionData action = action_of[name];
	for( int i = 1; i < number; ++i )
		_goals.emplace( action.name, std::make_pair<int, int>( std::move( number ), 0 ) ); // I don't understand why make_pair is waiting for &&int here

	if( !_exhaustive_inputs )
		rec_makeGoals( action, number );	
}

void BuilderBO::rec_makeGoals( const ActionData &action, int count )
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
