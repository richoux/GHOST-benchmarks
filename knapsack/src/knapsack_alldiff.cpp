#include "knapsack_alldiff.hpp"

KSAllDiff::KSAllDiff( const std::vector<ghost::Variable>& variables )
	: Constraint( variables )
{ }

double KSAllDiff::required_error( const std::vector<ghost::Variable*>& variables ) const
{
	/*
  // COP model
	for( int i = 0 ; i < static_cast<int>( variables.size() ) - 1 ; ++i )
		for( int j = i + 1 ; j < static_cast<int>( variables.size() ) ; ++j )
			if( variables[i]->get_value() == variables[j]->get_value() )
				return 1.0;
			
	return 0.0;
	/*/
	// EFOP model
	double count = 0.0;

	for( int i = 0 ; i < static_cast<int>( variables.size() ) - 1 ; ++i )
		for( int j = i + 1 ; j < static_cast<int>( variables.size() ) ; ++j )
			if( variables[i]->get_value() == variables[j]->get_value() )
			  ++count;

  return count;
  //*/
}
