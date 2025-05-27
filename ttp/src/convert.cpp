#include "convert.hpp"

void convert( int match, int number_teams, int &home_team, int &away_team )
{
	home_team = ( match / ( number_teams - 1 ) ) + 1;
	int shift = match / number_teams < home_team - 1 ? 1 : 2;
	away_team = ( match % ( number_teams - 1 ) ) + shift;
}
