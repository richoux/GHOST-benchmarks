#pragma once

#include <string>
#include <vector>

void convert( int match, int number_teams, int &home_team, int &away_team );

void extract_data_from_file( const std::string& filename,
                             int& number_variables,
                             std::vector< std::vector<double> >& matrix_distances );

int check_error_solution( const std::vector<int> &solution, int number_teams );

double check_cost_solution( const std::vector<int> &solution,
														int number_teams,
														const std::vector< std::vector<double> >& matrix_distances );
