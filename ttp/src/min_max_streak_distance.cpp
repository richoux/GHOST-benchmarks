#include <cmath>
#include "min_max_streak_distance.hpp"
#include "convert.hpp"

MinMaxStreakDistance::MinMaxStreakDistance( const std::vector<ghost::Variable>& variables,
																						int number_teams,
																						int number_weeks,
																						const std::vector< std::vector<double> >& matrix_distances )
	: Minimize( variables, "MinMaxStreakDistance" ),
	  _number_teams( number_teams ),
		_number_weeks( number_weeks ),
	  _matrix_distances( matrix_distances )
{ }

double MinMaxStreakDistance::required_cost( const std::vector<ghost::Variable*>& variables ) const
{
	double max_streak = 0.;
	std::vector<double> max_streaks( _number_teams, 0. );
	std::vector< std::vector<int> > weeks( _number_weeks );

	for( size_t match = 0 ; match < variables.size() ; ++match )
		weeks[variables[match]->get_value() - 1].push_back( match ); // variables[match]->get_value() - 1 because weeks start at 1
	
	int home = -1;
	int away = -1;

	for( int week = 0 ; week < _number_weeks ; ++week )
		for( auto& match: weeks[week] )
		{		
			convert( match, _number_teams, home, away );
			--home; // because team numbers start at 1, and we want indices
			--away;
			
			max_streaks[home] = 0.;
			max_streaks[away] += _matrix_distances[away][home];
			if( max_streak < max_streaks[away] )
				max_streak = max_streaks[away];
		}

	return max_streak;
}
