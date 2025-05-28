#pragma once

#include <ghost/objective.hpp>

class MinMaxStreakDistance : public ghost::Minimize
{
	int _number_teams;
	int _number_weeks;
	std::vector< std::vector<double> > _matrix_distances;
	
public:
	MinMaxStreakDistance( const std::vector<ghost::Variable>& variables,
												int number_teams,
												int number_weeks,
												const std::vector< std::vector<double> >& matrix_distances );
	
	double required_cost( const std::vector<ghost::Variable*>& variables ) const override;
};
