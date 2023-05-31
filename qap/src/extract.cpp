#include "extract.hpp"
#include <iostream>
#include <iomanip>

void extract_data_from_file( const std::string& filename,
                             int& number_variables,
                             std::vector< std::vector<int> >& matrix_distances,
                             std::vector< std::vector<int> >& matrix_flows )
{
	if( filename != "" )
	{		
		std::ifstream instance_file( filename );
		std::string line;
		std::string number;
		if( instance_file.is_open() )
		{
			std::getline( instance_file, line );
			number_variables = std::stoi( line );

			// empty line
			std::getline( instance_file, line );

			// get distance matrix
			for( int  i = 0 ; i < number_variables ; ++i )
			{
				matrix_distances.emplace_back( std::vector<int>( number_variables ) );
				std::getline( instance_file, line);
				std::stringstream ss( line );
				int j = 0;
				while( ss >> number )
				{
					matrix_distances[i][j] = std::stoi( number );
					++j;
				}					
			}

			// empty line
			std::getline( instance_file, line );

			// get flow matrix
			for( int  i = 0 ; i < number_variables ; ++i )
			{
				matrix_flows.emplace_back( std::vector<int>( number_variables ) );
				std::getline( instance_file, line);
				std::stringstream ss( line );
				int j = 0;
				while( ss >> number )
				{
					matrix_flows[i][j] = std::stoi( number );
					++j;
				}					
			}

			// std::cout << "Distances:\n";
			// for( int  i = 0 ; i < number_variables ; ++i )
			// {
			// 	for( int  j = 0 ; j < number_variables ; ++j )
			// 		std::cout << std::setw(6) << matrix_distances[i][j];
			// 	std::cout << std::endl;
			// }
			
			// std::cout << "\n\nFlows:\n";
			// for( int  i = 0 ; i < number_variables ; ++i )
			// {
			// 	for( int  j = 0 ; j < number_variables ; ++j )
			// 		std::cout << std::setw(6) << matrix_flows[i][j];
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
