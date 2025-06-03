#include <algorithm>

#include <iostream>
#include <iterator>

#include <ghost/global_constraints/all_different.hpp>

#include "builder_ttp.hpp"
#include "constraint_no_repeat.hpp"
#include "constraint_max_streak.hpp"
#include "misc.hpp"
#if defined TTP_OPT
#include "min_travel_distance.hpp"
#include "min_max_streak_distance.hpp"
#endif

BuilderTTP::BuilderTTP( int number_teams,
												const std::vector< std::vector<double> >& distances )
	: ghost::ModelBuilder( true ),
	  _number_teams( number_teams ),
		_number_matches( number_teams * ( number_teams - 1 ) ),
		_number_rounds( 2 * ( number_teams - 1 ) ),
		_distance_matrix( distances ),
		_homes( std::vector< std::vector<int> >( number_teams ) ),
		_aways( std::vector< std::vector<int> >( number_teams ) ),
		_pairs( std::vector< std::vector<int> >( _number_matches / 2 ) )
{
	int j = -1;
	int home_team = -1;
	int away_team = -1;
	
	// Prepare home variables
  for( int team = 0; team < number_teams; ++team ) // first team is 1, but we need to compute team-1, so let's start team at 0 here
	  for( int i = 0 ; i < number_teams - 1 ; ++i )
		  _homes[team].push_back( team * ( number_teams - 1 ) + i );
  
  // Prepare away variables
  for( int team = 0; team < number_teams; ++team ) // same
	  for( int i = 0 ; i < number_teams ; ++i )
			if( i != team )
			{
				j = i < team ? team - 1 : team;
				_aways[team].push_back( i * ( number_teams - 1 ) + j );
			}
			
  // Prepare pair variables
	int count = 0;
	for( int match = 0; match < _number_matches; ++match )
  {
		convert( match, number_teams, home_team, away_team );
		
		if( home_team < away_team )
		{
			_pairs[count].push_back( match );
			_pairs[count].push_back( match + ( _number_teams - 2 ) * ( away_team - home_team ) + 1 );
			++count;
		}
  }
}

BuilderTTP::BuilderTTP( int number_teams )
	: BuilderTTP( number_teams, {} )
{ }

void BuilderTTP::declare_variables()
{
	int round = 0; // to start with value 1, since i % matches_per_round == 0 when i=0
	int matches_per_round = _number_teams / 2;	

	std::vector<int> domain( _number_matches ); // {1,...,1,2,...,2,...,2(n-1),...,2(n-1)}, with 1,...,1 n/2 times
	for( int i = 0; i < _number_matches; ++i )
	{
		if( i % matches_per_round == 0 )
			++round;
		domain[i] = round;
	}

	create_n_variables( _number_matches, domain );

	for( int i = 0; i < _number_matches; ++i )
		variables[i].set_value( domain[i] );
}

void BuilderTTP::declare_constraints()
{
  // Double round robin
	for( int team = 0 ; team < _number_teams ; ++team ) // first team is 1, but the team index starts at 0
	{
		std::vector<int> team_matches;
		std::merge( _homes[team].begin(), _homes[team].end(), _aways[team].begin(), _aways[team].end(), std::back_inserter( team_matches ) );

		constraints.emplace_back( std::make_shared<ghost::global_constraints::AllDifferent>( team_matches ) );		
	}

	// No repeat
	for( auto& pair: _pairs )
		constraints.emplace_back( std::make_shared<NoRepeat>( pair ) );

	// Max streak
	for( int team = 0 ; team < _number_teams ; ++team ) // first team is 1, but the team index starts at 0
	{
		constraints.emplace_back( std::make_shared<MaxStreak>( _homes[team] ) );
		constraints.emplace_back( std::make_shared<MaxStreak>( _aways[team] ) );
	}
}

#if defined TTP_OPT
void BuilderTTP::declare_objective()
{
	objective = std::make_shared<MinTravelDistance>( variables,
																									 _number_teams,
																									 _number_rounds,
																									 _distance_matrix );
	// objective = std::make_shared<MinMaxStreakDistance>( variables,
	// 																										_number_teams,
	// 																										_number_rounds,
	// 																										_distance_matrix );
}
#endif
