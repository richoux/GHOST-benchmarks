#include "misc.hpp"

#include <iterator>
#include <fstream>
#include <sstream>
#include <iostream>
#include <iomanip>
#include <set>
#include <numeric>

void convert( int match, int number_teams, int &home_team, int &away_team )
{
	home_team = ( match / ( number_teams - 1 ) ) + 1;
	int shift = match / number_teams < home_team - 1 ? 1 : 2;
	away_team = ( match % ( number_teams - 1 ) ) + shift;
}

void extract_data_from_file( const std::string& filename,
                             int& number_variables,
                             std::vector< std::vector<double> >& matrix_distances )
{
	if( filename != "" )
	{		
		std::ifstream instance_file( filename );
		std::string line;
		std::string number;
		if( instance_file.is_open() )
		{
			// Get number of teams
			std::getline( instance_file, line );
			std::stringstream ss_num( line );
			ss_num >> number;
			number_variables = std::stoi( number );

			// Get distance matrix
			for( int i = 0 ; i < number_variables ; ++i )
			{
				std::getline( instance_file, line );
				std::stringstream ss( line );
				matrix_distances.emplace_back( std::vector<double>( number_variables ) );
				for( int j = 0 ; j < number_variables ; ++j )
				{
					ss >> number;
					matrix_distances[i][j] = std::stod( number );
				}
			}
		}
	}
}

int check_error_solution( const std::vector<int> &solution, int number_teams )
{
	int number_rounds = 2 * ( number_teams - 1 );
	int number_matches = number_teams * ( number_teams - 1 );
	std::set<int> teams;
	std::vector<int> streaks( number_teams, 0 );
	std::vector< std::vector<int> > rounds( number_rounds );
	int home = -1;
	int away = -1;

	int number_violated_constraints = 0;
	
	for( int match = 0 ; match < number_matches ; ++match )
		rounds[solution[match] - 1].push_back( match );

	for( int round = 0 ; round < number_rounds ; ++round )
	{
		teams.clear();
		for( auto& match: rounds[round] )
		{
			convert( match, number_teams, home, away );
			teams.insert( home );
			teams.insert( away );

			if( streaks[ home-1 ] <= 0 )
				streaks[ home-1 ] = 1;
			else
			{
				++streaks[ home-1 ];
				if( streaks[ home-1 ] >= 4 )
				{
					std::cout << "Error: team " << home << " has more than 3 games home (last on round " << round+1 << ")\n";
					++number_violated_constraints;
				}
			}
			
			if( streaks[ away-1 ] >= 0 )
				streaks[ away-1 ] = -1;
			else
			{
				--streaks[ away-1 ];
				if( streaks[ away-1 ] <= -4 )
				{
					std::cout << "Error: team " << away << " has more than 3 games away (last on round " << round+1 << ")\n";
					++number_violated_constraints;
				}
			}
		}

		if( teams.size() != number_teams )
		{
			std::cout << "Error: there are not " << number_teams << " team playing on round " << round+1
								<< "\nHere is the set of teams: ";
			std::copy( teams.begin(), teams.end(), std::ostream_iterator< int >(std::cout, " ") );
			std::cout << "\n";
			++number_violated_constraints;
		}
	}

	for( int match_a = 0 ; match_a < number_matches ; ++match_a )
	{
		convert( match_a, number_teams, home, away );
		if( home < away )
		{
			int match_b = match_a + ( number_teams - 2 ) * ( away - home ) + 1;
			if( std::abs( solution[ match_a ] - solution[ match_b ] ) <= 1 )
			{
				std::cout << "Error: teams " << home << " and " << away << " have two games in a row at rounds " << solution[ match_a ] << " and " << solution[ match_b ] << "\n";
				++number_violated_constraints;
			}
		}
	}

	return number_violated_constraints;
}

double check_cost_solution( const std::vector<int> &solution, int number_teams, const std::vector< std::vector<double> >& matrix_distances )
{
	double total_distance = 0.;
	int home = -1;
	int away = -1;
	
	int number_matches = number_teams * ( number_teams - 1 );
	int number_rounds = 2 * ( number_teams - 1 );
	std::vector< std::vector<int> > rounds( number_rounds );

	for( int match = 0 ; match < number_matches ; ++match )
		rounds[ solution[match] - 1 ].push_back( match ); // solution[match] - 1 because rounds start at 1

	std::vector<int> previously_at( number_teams);
	std::iota( previously_at.begin(), previously_at.end(), 0 );

	for( int round = 0 ; round < number_rounds ; ++round )
	{
		for( auto& match: rounds[round] )
		{		
			convert( match, number_teams, home, away );
			--home; // because team numbers start at 1, and we want indices
			--away;

			total_distance += matrix_distances[ previously_at[ away ] ][ home ];
			if( previously_at[ home ] != home )
			{
				total_distance += matrix_distances[ previously_at[ home ] ][ home ];
				previously_at[ home ] = home;
			}
			if( round == number_rounds - 1 )
				total_distance += matrix_distances[ home ][ away ]; // away returns home
			else
				previously_at[ away ] = home;
		}
	}
	
	return total_distance;
}
