#pragma once

#include <ghost/model_builder.hpp>
#include <map>

class BuilderTTP : public ghost::ModelBuilder
{
	int _number_teams;
	int _number_matches;
	int _number_weeks; // each team plays a match per week.
	std::vector< std::vector<double> > _distance_matrix;

	// variable scopes
	std::vector< std::vector<int> > _homes;
	std::vector< std::vector<int> > _aways;
	std::vector< std::vector<int> > _pairs;
	
public:
	BuilderTTP( int number_teams,
							const std::vector< std::vector<double> >& distances );
	
	void declare_variables() override;
	void declare_constraints() override;
	// void declare_objective() override;
};
