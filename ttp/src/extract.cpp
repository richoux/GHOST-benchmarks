#include "extract.hpp"
#include <iostream>
#include <iomanip>
#include <cmath>

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

void extract_solution_from_file( const std::string& filename,
                                 int& cost,
                                 std::vector<int>& solution )
{
	if( filename != "" )
	{		
		std::ifstream instance_file( filename );
		std::string line;
		std::string number;
		if( instance_file.is_open() )
		{
			std::getline( instance_file, line );
			std::stringstream ss_num( line );
			ss_num >> number;
			cost = std::stoi( number );
			
			std::getline( instance_file, line );
			std::stringstream ss_vec( line );
			while( ss_vec >> number )
			{
				solution.push_back( std::stoi( number ) );
			}
		}
	}
}
