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
			do
			{
				std::getline( instance_file, line);

				if( line.starts_with("DIMENSION") )
				{
					std::stringstream ss( line );
					ss >> number;
					ss >> number;
					if( number.starts_with(":") )
						ss >> number;

					number_variables = std::stoi( number );
				}				
			}
			while( !line.starts_with("NODE_COORD_SECTION") && !line.starts_with("EDGE_WEIGHT_SECTION") );

			if( line.starts_with("NODE_COORD_SECTION") )
			{			
				std::vector<double> x( number_variables );
				std::vector<double> y( number_variables );
			
				for( int i = 0 ; i < number_variables ; ++i )
				{
					// City number. We consider it starts by 1.
					std::getline( instance_file, line );
					std::stringstream ss( line );
					ss >> number;				
				
					// x coordinate
					ss >> number;
					x[i] = std::stod( number );

					// y coordinate
					ss >> number;
					y[i] = std::stod( number );
				}

				// compute the distance matrix
				for( int i = 0 ; i < number_variables ; ++i )
				{
					matrix_distances.emplace_back( std::vector<double>( number_variables ) );
					for( int j = 0 ; j < number_variables ; ++j )
					{
						if( i == j )
							matrix_distances[i][j] = 0.;
						else
							matrix_distances[i][j] = std::sqrt( ( x[i] - x[j] )*( x[i] - x[j] ) + ( y[i] - y[j] )*( y[i] - y[j] ) );
					}
				}
			}
			else // line should starts with EDGE_WEIGHT_SECTION
			{
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

			// std::cout << "Distances:\n";
			// for( int  i = 0 ; i < number_variables ; ++i )
			// {
			// 	for( int  j = 0 ; j < number_variables ; ++j )
			// 		std::cout << std::setw(6) << matrix_distances[i][j];
			// 	std::cout << std::endl;
			// }
			// std::cout << std::endl;
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
