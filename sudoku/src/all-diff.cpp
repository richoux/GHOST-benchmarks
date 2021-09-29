#include <cmath>
#include <algorithm>
#include <iostream>

#include "all-diff.hpp"

double binomial_with_2( int value )
{
	if( value <= 1 )
		return 0;
	
	if( value % 2 == 0 )
		return static_cast<double>( value - 1 ) * ( value / 2 );
	else
		return static_cast<double>( value ) * ( ( value - 1 ) / 2 );
}

inline double binomial_with_2_diff_minus_1( int value )
{
	return binomial_with_2( value - 1 ) - binomial_with_2( value );
}

inline double binomial_with_2_diff_plus_1( int value )
{
	return binomial_with_2( value + 1 ) - binomial_with_2( value );
}

AllDiff::AllDiff( const vector<int>& variables_index )
	: Constraint( variables_index ),
	  _count( vector<int>( variables_index.size() ) )
{ }

// SOFT_ALLDIFF error function (Petit et al. 2001)
double AllDiff::required_error( const vector<Variable*>& variables ) const
{
	double counter = 0;

	std::fill( _count.begin(), _count.end(), 0 );

	for( auto v : variables )
		++_count[ v->get_value() - 1 ];
	
	for( auto c : _count )
		if( c > 1 )
			counter += binomial_with_2( c );

	return counter;
}

double AllDiff::optional_delta_error( const vector<Variable*>& variables, const vector<int>& variable_indexes, const vector<int>& candidate_values ) const
{
	double diff = 0.0;
	auto copy_count( _count );

	// std::cout << "In AllDiff::expert_delta_error, constraint num. " << this->get_id()  << "\n";
	// for( int i = 0 ; i < static_cast<int>( variable_indexes.size() ) ; ++i )
	// 	std::cout << "v[" << variable_indexes[ i ] << "]=" << variables[ variable_indexes[ i ] ]->get_value() << ", candidate=" << candidate_values[ i ] << "\n";

	for( int i = 0 ; i < static_cast<int>( variable_indexes.size() ) ; ++i )
	{
		// std::cout << "Before change: count[" << variables[ variable_indexes[ i ] ]->get_value() << "]=" << copy_count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ]
		//           << ", count[" << candidate_values[ i ] << "]=" << copy_count[ candidate_values[ i ] - 1 ] << "\n";

		--copy_count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ];
		++copy_count[ candidate_values[ i ] - 1 ];

		// std::cout << "After change: count[" << variables[ variable_indexes[ i ] ]->get_value() << "]=" << copy_count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ]
		//           << ", count[" << candidate_values[ i ] << "]=" << copy_count[ candidate_values[ i ] - 1 ] << "\n";
	}
	
	for( int i = 0 ; i < static_cast<int>( variable_indexes.size() ) ; ++i )
	{
		if( _count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ] != copy_count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ] )
		{
			// std::cout << "Before change: error[" << variables[ variable_indexes[ i ] ]->get_value() << "]=" << binomial_with_2( _count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ] )
			//           << ", after change: error[" << variables[ variable_indexes[ i ] ]->get_value() << "]=" << binomial_with_2( copy_count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ] ) << "\n";

			diff += ( binomial_with_2( copy_count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ] ) - binomial_with_2( _count[ variables[ variable_indexes[ i ] ]->get_value() - 1 ] ) );
		}
		if( _count[ candidate_values[ i ] - 1 ] != copy_count[ candidate_values[ i ] - 1 ] )
		{
			// std::cout << "Before change: error[" << candidate_values[ i ] << "]=" << binomial_with_2( _count[ candidate_values[ i ] - 1 ] )
			//           << ", after change: error[" << candidate_values[ i ] << "]=" << binomial_with_2( copy_count[ candidate_values[ i ] - 1 ] ) << "\n";
			
			diff += ( binomial_with_2( copy_count[ candidate_values[ i ] - 1 ] ) - binomial_with_2( _count[ candidate_values[ i ] - 1 ] ) );
		}
	}

	// std::cout << "Total diff=" << diff << "\n";
	return diff;
}

void AllDiff::conditional_update_data_structures( const vector<Variable*>& variables, int variable_index, int new_value )
{
	--_count[ variables[ variable_index ]->get_value() - 1 ];
	++_count[ new_value - 1 ];
}
